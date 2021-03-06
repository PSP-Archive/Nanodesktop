/*  GNU Ocrad - Optical Character Recognition program
    Copyright (C) 2003, 2004, 2005, 2006, 2007 Antonio Diaz Diaz.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>

#include "common.h"
#include "rectangle.h"
#include "track.h"
#include "bitmap.h"
#include "blob.h"
#include "character.h"
#include "page_image.h"
#include "textline.h"
#include "textblock.h"
#include "textpage.h"


namespace {

struct Zone
  {
  Rectangle rectangle;
  std::vector< std::vector< Blob * > > blobp_matrix;
  Zone( const Rectangle & r ) : rectangle( r ) {}
  };


int blobs_in_zone( const std::vector< Zone > & zone_vector, int i ) throw()
  {
  const std::vector< std::vector< Blob * > > & blobp_matrix = zone_vector[i].blobp_matrix;
  int sum = 0;
  for( unsigned int cut = 0; cut < blobp_matrix.size(); ++cut )
    sum += blobp_matrix[cut].size();
  return sum;
  }


int blobs_in_page( const std::vector< Zone > & zone_vector ) throw()
  {
  int sum = 0;
  for( unsigned int i = 0; i < zone_vector.size(); ++i )
    sum += blobs_in_zone( zone_vector, i );
  return sum;
  }


void bprint( const std::vector< Zone > & zone_vector, FILE * outfile ) throw()
  {
//  std::fprintf( outfile, "page size %dw x %dh\n", width(), height() );
  std::fprintf( outfile, "total zones in page %d\n", (int)zone_vector.size() );
  std::fprintf( outfile, "total blobs in page %d\n\n", blobs_in_page( zone_vector ) );
  for( unsigned int zindex = 0; zindex < zone_vector.size(); ++zindex )
    {
    const Rectangle & r = zone_vector[zindex].rectangle;
    const std::vector< std::vector< Blob * > > & blobp_matrix = zone_vector[zindex].blobp_matrix;

    std::fprintf( outfile, "zone %d of %d\n", zindex + 1, (int)zone_vector.size() );
    std::fprintf( outfile, "zone size %dw x %dh\n", r.width(), r.height() );
    std::fprintf( outfile, "total cuts in zone %d\n", (int)blobp_matrix.size() );
    std::fprintf( outfile, "total blobs in zone %d\n\n", blobs_in_zone( zone_vector, zindex ) );

    for( unsigned int cut = 0; cut < blobp_matrix.size(); ++cut )
      {
      std::fprintf( outfile, "cut %d blobs %d\n", cut + 1, (int)blobp_matrix[cut].size() );
      for( unsigned int i = 0; i < blobp_matrix[cut].size(); ++i )
      blobp_matrix[cut][i]->print( outfile );
      }
    }
  }


inline void join_blobs( std::vector< Blob * > & blobp_vector,
                         std::vector< Blob * > & v1,
                         std::vector< Blob * > & v2,
                         Blob * p1, Blob * p2, int i ) throw()
  {
  if( p1->top() > p2->top() )
    {
    Blob * temp = p1; p1 = p2; p2 = temp;
    std::replace( v2.begin(), v2.begin() + i + 1, p2, p1 );
    }
  else std::replace( v1.begin() + i, v1.end(), p2, p1 );

  i = blobp_vector.size();
  while( --i >= 0 && blobp_vector[i] != p2 );
  if( i < 0 ) Ocrad::internal_error( "join_blobs, lost blob" );
  blobp_vector.erase( blobp_vector.begin() + i );

  p1->add_bitmap( *p2 );
  delete p2;
  }


void ignore_abnormal_blobs( std::vector< Blob * > & blobp_vector ) throw()
  {
  for( int i = blobp_vector.size() - 1; i >= 0; --i )
    {
    Blob & b = *blobp_vector[i];
    if( b.height() > 35 * b.width() || b.width() > 25 * b.height() )
      { delete blobp_vector[i];
      blobp_vector.erase( blobp_vector.begin() + i ); }
    }
  }


void ignore_small_blobs( std::vector< Blob * > & blobp_vector ) throw()
  {
  int to = 0, blobs = blobp_vector.size();
  for( int from = 0; from < blobs; ++from )
    {
    Blob * p = blobp_vector[from];
    if( p->height() > 4 || p->width() > 4 ||
        ( ( p->height() > 2 || p->width() > 2 ) && p->area() > 5 ) )
      { blobp_vector[from] = blobp_vector[to]; blobp_vector[to] = p; ++to; }
    }
  if( to < blobs )
    {
    for( int i = to; i < blobs; ++i ) delete blobp_vector[i];
    blobp_vector.erase( blobp_vector.begin() + to, blobp_vector.end() );
    }
  }


void ignore_wide_blobs( Zone & zone ) throw()
  {
  const Rectangle & r = zone.rectangle;
  std::vector< std::vector< Blob * > > & blobp_matrix = zone.blobp_matrix;

  for( unsigned int cut = 0; cut < blobp_matrix.size(); ++cut )
    {
    bool frame_found = false;
    for( unsigned int i = 0; i < blobp_matrix[cut].size(); )
      {
      std::vector< Blob * > & blobp_vector = blobp_matrix[cut];
      Blob & b = *blobp_vector[i];
      if( 2 * b.width() < r.width() ) { ++i; continue; }
      blobp_vector.erase( blobp_vector.begin() + i );
      if( 4 * b.area() <= 3 * b.size() )
        {
        if( 4 * b.area() <= b.size() )
          { frame_found = true; delete &b; continue; }
        b.find_holes();
        int j = -1;
        if( b.holes() < std::min( b.height(), b.width() ) )
          for( j = 0; j < b.holes(); ++j )
            if( 4 * b.hole( j ).size() >= b.size() &&
                4 * b.hole( j ).area() >= b.size() )
              break;
        if( j >= 0 && j < b.holes() )
          { frame_found = true; delete &b; continue; }
        }
      // image, not frame
      if( 5 * b.width() > 4 * r.width() && 5 * b.height() > 4 * r.height() )
        {
        for( unsigned int j = 0; j < blobp_vector.size(); ++j )
          delete blobp_vector[j];
        blobp_vector.clear(); delete &b; break;
        }
      for( int j = blobp_vector.size() - 1; j >= 0; --j )
        {
        const Blob & b2 = *blobp_vector[j];
        if( b.includes( b2 ) )
          { delete &b2; blobp_vector.erase( blobp_vector.begin() + j ); }
        else if( b2.top() < b.top() ) break;
        }
      delete &b;
      }
    if( frame_found )	// Make cuts from blobs inside deleted frame(s)
      {
      int bottom = 0;
      for( unsigned int i = 0; i < blobp_matrix[cut].size(); ++i )
        {
        std::vector< Blob * > & blobp_vector = blobp_matrix[cut];
        const Blob & b = *blobp_vector[i];
        if( b.bottom() > bottom )
          {
          int old_bottom = bottom; bottom = b.bottom();
          if( b.top() > old_bottom && i > 0 )
            {
            std::vector< Blob * > new_blobp_vector( blobp_vector.begin() + i, blobp_vector.end() );
            blobp_vector.erase( blobp_vector.begin() + i, blobp_vector.end() );
            blobp_matrix.insert( blobp_matrix.begin() + cut + 1, new_blobp_vector );
            ++cut; i = 0;
            }
          }
        }
      }
    }
  }


void remove_top_bottom_noise( std::vector< Blob * > & blobp_vector ) throw()
  {
  int blobs = blobp_vector.size();
  for( int i = 0; i < blobs; ++i )
    {
    Blob & b = *blobp_vector[i];
    if( b.height() < 11 ) continue;

    int c = 0;
    for( int col = b.left(); col <= b.right(); ++col )
      if( b.get_bit( b.top(), col ) && ++c > 1 ) break;
    if( c <= 1 ) b.top( b.top() + 1 );

    c = 0;
    for( int col = b.left(); col <= b.right(); ++col )
      if( b.get_bit( b.bottom(), col ) && ++c > 1 ) break;
    if( c <= 1 ) b.bottom( b.bottom() - 1 );
    }
  }


void remove_left_right_noise( std::vector< Blob * > & blobp_vector ) throw()
  {
  int blobs = blobp_vector.size();
  for( int i = 0; i < blobs; ++i )
    {
    Blob & b = *blobp_vector[i];
    if( b.width() < 6 ) continue;

    int c = 0;
    for( int row = b.top(); row <= b.bottom(); ++row )
      if( b.get_bit( row, b.left() ) && ++c > 1 ) break;
    if( c <= 1 ) b.left( b.left() + 1 );

    c = 0;
    for( int row = b.top(); row <= b.bottom(); ++row )
      if( b.get_bit( row, b.right() ) && ++c > 1 ) break;
    if( c <= 1 ) b.right( b.right() - 1 );
    }
  }


void find_holes( std::vector< Zone > & zone_vector ) throw()
  {
  for( unsigned int zi = 0; zi < zone_vector.size(); ++zi )
    {
    std::vector< std::vector< Blob * > > & blobp_matrix = zone_vector[zi].blobp_matrix;
    for( unsigned int bmi = 0; bmi < blobp_matrix.size(); ++bmi )
      {
      std::vector< Blob * > & blobp_vector = blobp_matrix[bmi];
      for( unsigned int bvi = 0; bvi < blobp_vector.size(); ++bvi )
        blobp_vector[bvi]->find_holes();
      }
    }
  }


void scan_page( const Page_image & page_image,
                std::vector< Zone > & zone_vector, const int debug_level ) throw()
  {
  for( int zindex = 0; zindex < page_image.zones(); ++zindex )
    {
    zone_vector.push_back( Zone( page_image.rectangle( zindex ) ) );
    const Rectangle & re = zone_vector[zindex].rectangle;
    const int zthreshold = page_image.threshold( zindex );
    std::vector< std::vector< Blob * > > & blobp_matrix = zone_vector[zindex].blobp_matrix;
    std::vector< Blob * > blobp_vector;
    std::vector< Blob * > old_data( re.width(), (Blob *) 0 );
    std::vector< Blob * > new_data( re.width(), (Blob *) 0 );

    for( int row = re.top() + 1; row <= re.bottom(); ++row )
      {
      bool blank_row = true;
      old_data.swap( new_data );
      for( int col = re.left() + 1; col < re.right(); ++col )
        {
        const int dcol = col - re.left();
        if( !page_image.get_bit( row, col, zthreshold ) )
          new_data[dcol] = 0;			// white point
        else					// black point
          {
          blank_row = false;
          Blob *p;
          Blob *lp  = new_data[dcol-1];
          Blob *ltp = old_data[dcol-1];
          Blob *tp  = old_data[dcol];
          Blob *rtp = old_data[dcol+1];
          if( lp )       { p = lp;  p->add_point( row, col ); }
          else if( ltp ) { p = ltp; p->add_point( row, col ); }
          else if( tp )  { p = tp;  p->add_point( row, col ); }
          else if( rtp ) { p = rtp; p->add_point( row, col ); }
          else
            {
            p = new Blob( col, row, col, row );
            p->set_bit( row, col, true );
            blobp_vector.push_back( p );
            }
          new_data[dcol] = p;
          if( rtp && p != rtp )
            join_blobs( blobp_vector, old_data, new_data, p, rtp, dcol );
          }
        }
      if( blank_row && blobp_vector.size() )
        {
        blobp_matrix.push_back( std::vector< Blob * >() );
        blobp_vector.swap( blobp_matrix.back() );
        }
      }
    if( blobp_vector.size() )
      {
      blobp_matrix.push_back( std::vector< Blob * >() );
      blobp_vector.swap( blobp_matrix.back() );
      }
    }

  if( debug_level <= 99 )
    for( unsigned int zindex = 0; zindex < zone_vector.size(); ++zindex )
      {
      ignore_wide_blobs( zone_vector[zindex] );
      std::vector< std::vector< Blob * > > & blobp_matrix = zone_vector[zindex].blobp_matrix;
      for( unsigned int cut = 0; cut < blobp_matrix.size(); ++cut )
        {
        std::vector< Blob * > & blobp_vector = blobp_matrix[cut];
        ignore_small_blobs( blobp_vector );
        ignore_abnormal_blobs( blobp_vector );
        remove_top_bottom_noise( blobp_vector );
        remove_left_right_noise( blobp_vector );
        }
      }

  for( int zindex = zone_vector.size() - 1; zindex >= 0; --zindex )
    {
    std::vector< std::vector< Blob * > > & blobp_matrix = zone_vector[zindex].blobp_matrix;
    for( int cut = blobp_matrix.size() - 1; cut >= 0; --cut )
      if( !blobp_matrix[cut].size() )
        blobp_matrix.erase( blobp_matrix.begin() + cut );
    if( !blobp_matrix.size() )
      zone_vector.erase( zone_vector.begin() + zindex );
    }

  find_holes( zone_vector );
  }

} // end namespace


Textpage::Textpage( const Page_image & page_image,
                    const char * filename, const Control & control ) throw()
  : Rectangle( page_image ), name( filename )
  {
  const int debug_level = control.debug_level;
  if( debug_level < 0 || debug_level > 100 ) return;

  std::vector< Zone > zone_vector;
  scan_page( page_image, zone_vector, debug_level );

  if( debug_level >= 98 )
    {
    if( control.outfile ) bprint( zone_vector, control.outfile );
    return;
    }
  if( debug_level > 95 || ( debug_level > 89 && debug_level < 94 ) ) return;

  // build a Textblock for every zone with text
  for( unsigned int i = 0; i < zone_vector.size(); ++i )
    {
    Textblock * tbp = new Textblock( zone_vector[i].rectangle,
                                     zone_vector[i].blobp_matrix );
    if( debug_level < 90 )
      tbp->recognize( control.charset, control.filter );
    if( tbp->textlines() ) tbpv.push_back( tbp );
    else delete tbp;
    }
  if( debug_level == 0 )
    {
    if( control.outfile ) print( control );
    if( control.exportfile ) xprint( control );
    return;
    }
  if( !control.outfile ) return;
  if( debug_level >= 86 )
    {
    bool graph = ( debug_level >= 88 );
    bool recursive = ( debug_level & 1 );
    for( int i = 0; i < textblocks(); ++i )
      tbpv[i]->dprint( control, graph, recursive );
    return;
    }
  if( debug_level > 77 ) return;
  if( debug_level >= 70 )
    {
    Page_image tmp( page_image );
    if( ( debug_level - 70 ) & 1 )	// mark zones
      {
      for( unsigned int i = 0; i < zone_vector.size(); ++i )
        tmp.draw_rectangle( zone_vector[i].rectangle );
      }
    if( ( debug_level - 70 ) & 2 )	// mark lines
      {
      for( int i = 0; i < textblocks(); ++i ) tbpv[i]->lmark( tmp );
      }
    if( ( debug_level - 70 ) & 4 )	// mark characters
      {
      for( int i = 0; i < textblocks(); ++i ) tbpv[i]->cmark( tmp );
      }
    tmp.save( control.outfile, control.filetype );
    return;
    }
  }


Textpage::~Textpage() throw()
  {
  for( int i = textblocks() - 1; i >= 0; --i ) delete tbpv[i];
  }

/*
const Textblock & Textpage::textblock( int i ) const throw()
  {
  if( i < 0 || i >= textblocks() )
    Ocrad::internal_error( "Textpage::textblock, index out of bounds" );
  return *(tbpv[i]);
  }
*/

void Textpage::print( const Control & control ) const throw()
  {
  if( control.outfile )
    for( int i = 0; i < textblocks(); ++i )
      tbpv[i]->print( control );
  }


void Textpage::xprint( const Control & control ) const throw()
  {
  if( !control.exportfile ) return;

  std::fprintf( control.exportfile, "source file %s\n", name.c_str() );
  std::fprintf( control.exportfile, "total text blocks %d\n", textblocks() );

  for( int i = 0; i < textblocks(); ++i )
    {
    const Textblock & tb = *(tbpv[i]);
    std::fprintf( control.exportfile, "text block %d %d %d %d %d\n", i + 1,
                  tb.left(), tb.top(), tb.width(), tb.height() );
    tb.xprint( control );
    }
  }

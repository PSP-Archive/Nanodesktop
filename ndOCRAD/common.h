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

namespace Ocrad {

extern bool verbose;

void internal_error( const char * msg ) throw() __attribute__ ((noreturn));
bool similar( int a, int b, int percent_dif, int abs_dif = 1 ) throw();

} // end namespace Ocrad


class Charset
  {
  int _charset;

public:
  enum Value { ascii = 1, iso_8859_9 = 2, iso_8859_15 = 4 };

  Charset() throw() : _charset( 0 ) {}
  bool enable( const char * name ) throw();
  bool enabled( Value cset ) const throw();
  bool only( Value cset ) const throw();
  void show_error( const char * program_name, const char * arg ) const throw();
  };


class Filter
  {
public:
  enum Type { none, letters, letters_only, numbers, numbers_only };
private:
  Type _type;

public:
  Filter() throw() : _type( none ) {}
  bool set( const char * name ) throw();
  Type type() const throw() { return _type; }
  void show_error( const char * program_name, const char * arg ) const throw();
  };


class Transformation
  {
public:
  enum Type { none, rotate90, rotate180, rotate270,
              mirror_lr, mirror_tb, mirror_d1, mirror_d2 };
private:
  Type _type;

public:
  Transformation() throw() : _type( none ) {}
  bool set( const char * name ) throw();
  Type type() const throw() { return _type; }
  void show_error( const char * program_name, const char * arg ) const throw();
  };


struct Control
  {
  enum Format { byte, utf8 };

  Charset charset;
  Filter filter;
  Format format;
  FILE *outfile, *exportfile;
  int debug_level;
  char filetype;

  Control() throw()
    : format( byte ), outfile( stdout ), exportfile( 0 ),
      debug_level( 0 ), filetype( '4' ) {}

  bool set_format( const char * name ) throw();
  };

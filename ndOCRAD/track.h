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

class Vrhomboid			// Rhomboid with two vertical sides.
  {
  int _left, _lvcenter, _right, _rvcenter, _height;

public:
  Vrhomboid( const int l, const int lc, const int r, const int rc, const int h ) throw();

  int left()     const throw() { return _left;     }
  int lvcenter() const throw() { return _lvcenter; }
  int right()    const throw() { return _right;    }
  int rvcenter() const throw() { return _rvcenter; }
  int height()   const throw() { return _height;   }
  int width()    const throw() { return _right - _left + 1; }
  int size()     const throw() { return _height * width(); }

  void left( const int l ) throw();
  void lvcenter( const int lc ) throw() { _lvcenter = lc; }
  void right( const int r ) throw();
  void rvcenter( const int rc ) throw() { _rvcenter = rc; }
  void height( const int h ) throw();
  void extend_left( const int l ) throw();
  void extend_right( const int r ) throw();

  int bottom( const int col ) const throw() { return vcenter( col ) + ( _height / 2 ); }
  int top( const int col ) const throw() { return bottom( col ) - _height + 1; }
  int vcenter( const int col ) const throw();

  bool includes( const Rectangle & r ) const throw();
  bool includes( const int row, const int col ) const throw();
  };


class Track		// vector of Vrhomboids tracking a Textline.
  {
  std::vector< Vrhomboid > data;

public:
  Track() throw() {}
  Track( const Track & t ) throw() : data( t.data ) {}
  Track & operator=( const Track & t ) throw()
    { if( this != &t ) { data = t.data; } return *this; }

  void set_track( const std::vector< Rectangle > & rectangle_vector ) throw();

  int segments() const throw() { return data.size(); }
  int height() const throw() { return data.size() ? data[0].height() : 0; }
  int left() const throw() { return data.size() ? data[0].left() : 0; }
  int right() const throw() { return data.size() ? data.back().right() : 0; }

  int bottom( const int col ) const throw();
  int top( const int col ) const throw();
  int vcenter( const int col ) const throw();

  bool includes( const Rectangle & r ) const throw();
  bool includes( const int row, const int col ) const throw();
  };

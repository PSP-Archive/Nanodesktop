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

class Features
  {
  const Blob * _blob;		// Blob to witch this features belong
  mutable int _hbars, _vbars;
  mutable bool hscan_valid, vscan_valid;
  mutable std::vector< Rectangle > _hbar, _vbar;
  mutable std::vector< int > _hscan, _vscan;

public:
  mutable Profile lp, tp, rp, bp, hp, wp;

  Features( const Blob & b ) throw();

//  const Blob * blob() const throw() { return _blob; }

  const Rectangle & hbar( int i ) const throw() { return _hbar[i]; }
  const Rectangle & vbar( int i ) const throw() { return _vbar[i]; }
  int hbars() const throw();
  int vbars() const throw();

  const std::vector< int > & hscan() const throw();
  const std::vector< int > & vscan() const throw();

  int test_235Esz( const Charset & charset ) const throw();
  int test_49ARegpq( const Rectangle & charbox ) const throw();
  int test_4ADQao( const Charset & charset, const Rectangle & charbox ) const throw();
  int test_6abd( const Charset & charset ) const throw();
  int test_CEFIJLlT( const Charset & charset ) const throw();
  int test_c() const throw();
  int test_frst( const Rectangle & charbox ) const throw();
  int test_G() const throw();
  int test_HKMNUuvwYy( const Rectangle & charbox ) const throw();
  int test_hknwx( const Rectangle & charbox ) const throw();
  int test_s_cedilla() const throw();

  bool test_comma() const throw();
  int test_easy( const Rectangle & charbox ) const throw();
  int test_line( const Rectangle & charbox ) const throw();
  int test_solid( const Rectangle & charbox ) const throw();
  int test_misc( const Rectangle & charbox ) const throw();
  };

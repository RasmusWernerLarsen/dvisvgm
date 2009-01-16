/***********************************************************************
** FontManager.h                                                      **
**                                                                    **
** This file is part of dvisvgm -- the DVI to SVG converter           **
** Copyright (C) 2005-2007 Martin Gieseking <martin.gieseking@uos.de> **
**                                                                    **
** This program is free software; you can redistribute it and/or      **
** modify it under the terms of the GNU General Public License        **
** as published by the Free Software Foundation; either version 2     **
** of the License, or (at your option) any later version.             **
**                                                                    **
** This program is distributed in the hope that it will be useful,    **
** but WITHOUT ANY WARRANTY; without even the implied warranty of     **
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the      **
** GNU General Public License for more details.                       **
**                                                                    **
** You should have received a copy of the GNU General Public License  **
** along with this program; if not, write to the Free Software        **
** Foundation, Inc., 51 Franklin Street, Fifth Floor,                 **
** Boston, MA 02110-1301, USA.                                        **
***********************************************************************/
// $Id$

#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <map>
#include <ostream>
#include <set>
#include <string>
#include <stack>
#include <vector>
#include "types.h"

using std::map;
using std::ostream;
using std::set;
using std::string;
using std::stack;
using std::vector;

class FileFinder;
class Font;
class TFM;
class VirtualFont;

/** This class provides methods for an easy DVI font handling. 
 *  DVI and VF files use local font numbers to reference fonts. For SVG output
 *  we need a single list with unique IDs of all physical fonts. Characters of
 *  virtual fonts are completely replaced by their DVI description so they don't
 *  appear anywhere in the output. */
class FontManager
{
	typedef map<UInt32,int> Num2IdMap;
	typedef map<string,int> Name2IdMap;
	typedef map<VirtualFont*,Num2IdMap> VfNum2IdMap;
	typedef map<VirtualFont*, UInt32> VfFirstFontMap;
	typedef stack<VirtualFont*> VfStack;
   public:
      FontManager ();
      ~FontManager ();
		int registerFont (UInt32 fontnum, string fontname, UInt32 checksum, double dsize, double scale);
		Font* getFont (int n) const;
		Font* getFont (string name) const;
		Font* getFontById (int id) const;
		VirtualFont* getVF () const;
		int fontID (int n) const;
		int fontID (const Font *font) const;
		int fontID (string name) const;
		int fontnum (int id) const;
		int vfFirstFontNum (VirtualFont *vf) const;
		void enterVF (VirtualFont *vf);
		void leaveVF ();
		void assignVfChar (int c, vector<UInt8> *dvi);
		const vector<Font*>& getFonts () const {return fonts;}
		ostream& write (ostream &os, Font *font=0, int level=0);

   private:
		vector<Font*> fonts;     ///< all registered Fonts (indexed by fontID)
		Num2IdMap     num2id;    ///< DVI font number -> fontID
		Name2IdMap    name2id;   ///< fontname -> fontID
		VfNum2IdMap   vfnum2id;
		VfStack       vfStack;   ///< stack of currently processed virtual fonts
		VfFirstFontMap vfFirstFontMap; ///< VF -> local font number of first font defined in VF
};

#endif
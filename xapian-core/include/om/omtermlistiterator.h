/** \file omtermlistiterator.h
 * \brief Classes for iterating through term lists
 */
/* ----START-LICENCE----
 * Copyright 1999,2000,2001 BrightStation PLC
 * Copyright 2002 Ananova Ltd
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 * -----END-LICENCE-----
 */

#ifndef OM_HGUARD_OMTERMLISTITERATOR_H
#define OM_HGUARD_OMTERMLISTITERATOR_H

#include <iterator>
#include <string>
#include "om/omtypes.h"

class OmDatabase;
namespace Xapian {
class PositionListIterator;
}

/** An iterator pointing to items in a list of terms.
 */
class OmTermIterator {
    private:
	// friend classes which need to be able to construct us
	friend class OmDatabase;
	friend class OmDocument;

    public:
	class Internal;
	/// @internal Reference counted internals.
	Internal *internal;

        friend bool operator==(const OmTermIterator &a,
			       const OmTermIterator &b);

    public:
	// FIXME: better if this was private...
	OmTermIterator(Internal *internal_);

	/// Default constructor - for declaring an uninitialised iterator
	OmTermIterator();

	/// Destructor
        ~OmTermIterator();

        /** Copying is allowed.  The internals are reference counted, so
	 *  copying is also cheap.
	 */
	OmTermIterator(const OmTermIterator &other);

        /** Assignment is allowed.  The internals are reference counted,
	 *  so assignment is also cheap.
	 */
	void operator=(const OmTermIterator &other);

	std::string operator *() const;

	OmTermIterator & operator++();

	void operator++(int);

	// extra method, not required for an input_iterator
	void skip_to(const std::string & tname);

	om_termcount get_wdf() const;
	om_doccount get_termfreq() const;

    	// allow iteration of positionlist for current document
	Xapian::PositionListIterator positionlist_begin();
	Xapian::PositionListIterator positionlist_end();
    
	/** Returns a string describing this object.
	 *  Introspection method.
	 */
	std::string get_description() const;

	/// Allow use as an STL iterator
	//@{
	typedef std::input_iterator_tag iterator_category;
	typedef std::string value_type;
	typedef om_termcount_diff difference_type;
	typedef std::string * pointer;
	typedef std::string & reference;
	//@}
};

inline bool
operator!=(const OmTermIterator &a, const OmTermIterator &b)
{
    return !(a == b);
}

#endif /* OM_HGUARD_OMTERMLISTITERATOR_H */

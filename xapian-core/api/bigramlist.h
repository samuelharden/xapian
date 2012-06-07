/** @file bigramlist.h
 * @brief Abstract base class for bigramlist.
 */
/* Copyright (C) 2012 Gaurav Arora
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
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

#ifndef XAPIAN_INCLUDED_BIGRAMLIST_H
#define XAPIAN_INCLUDED_BIGRAMLIST_H

#include <string>

#include "xapian/intrusive_ptr.h"
#include <xapian/types.h>
#include <xapian/bigramiterator.h>

namespace Xapian {
    namespace Internal {
	class ExpandStats;
    }
}

/// Abstract base class for termlists.
class Xapian::BigramIterator::Internal : public Xapian::Internal::intrusive_base {
    /// Don't allow assignment.
    void operator=(const Internal &);

    /// Don't allow copying.
    Internal(const Internal &);

  protected:
    /// Only constructable as a base class for derived classes.
    Internal() { }

  public:
    /** We have virtual methods and want to be able to delete derived classes
     *  using a pointer to the base class, so we need a virtual destructor.
     */
    virtual ~Internal();

    /// Return approximate size of this termlist.
    virtual Xapian::termcount get_approx_size() const = 0;

    /// Collate weighting information for the current bigram.
    virtual void accumulate_stats(Xapian::Internal::ExpandStats & stats) const;

    /// Return the bigramname at the current position.
    virtual std::string get_bigramname() const = 0;

    /// Return the wdf for the bigram at the current position.
    virtual Xapian::termcount get_wdf() const = 0;

    /// Return the bigram frequency for the term at the current position.
    virtual Xapian::doccount get_termfreq() const = 0;

    /** Return the collection frequency for the bigram at the current position.
     *
     *  This method is only implemented for subclasses of AllTermsList
     *  (and isn't currently used).
     */
    virtual Xapian::termcount get_collection_freq() const;

    /** Advance the current position to the next bigram in the bigramlist.
     *
     *  The list starts before the first bigram in the list, so next()
     *  must be called before any methods which need the context of
     *  the current position.
     *
     *  @return	If a non-NULL pointer is returned, then the caller should
     *		substitute the returned pointer for its pointer to us, and then
     *		delete us.  This "pruning" can only happen for a non-leaf
     *		subclass of this class.
     */
    virtual Internal * next() = 0;

    /** Skip forward to the specified bigram.
     *
     *  If the specified term isn't in the list, position ourselves on the
     *  first term after tname (or at_end() if no terms after tname exist).
     */
    virtual Internal * skip_to(const std::string &bterm) = 0;

    /// Return true if the current position is past the last bigram in this list.
    virtual bool at_end() const = 0;

};

// In the external API headers, this class is Xapian::BigramIterator::Internal,
typedef Xapian::BigramIterator::Internal BigramList;

#endif // XAPIAN_INCLUDED_BIGRAMLIST_H

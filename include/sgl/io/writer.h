/* Copyright (c) 2015
 * 
 * Bezborodov, V.A. vyacheslav.bezborodov@gmail.com
 * Golodov, V.A. avaksa@gmail.com
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#pragma once /** @file writer.h View writer. */

#include <sgl/view/view.h>

namespace sgl
{
    namespace io
    {
        /**
         * Represents an interface for sgl::view::view writers.
         * 
         * @ingroup io
         */
        class writer
        {
        public:
            virtual ~writer() {}

            /**
             * Writes a sgl::view::view.
             * 
             * @param view - a sgl::view::view.
             */
            virtual void write(sgl::view::const_view_t& view) = 0;
        };

        typedef std::shared_ptr<writer> writer_t;   ///< writer type.
    } // io
} // sgl

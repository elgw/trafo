#pragma once

/*    Copyright (C) 2020 Erik L. G. Wernersson
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/* Floating point-only table stored in row-major format.
*/

#define FTAB_VERSION_MAJOR 0
#define FTAB_VERSION_MINOR 1
#define FTAB_VERSION_PATCH 0

#include <stdint.h>
#include <stdio.h>


/* row-major table */
typedef struct {
    float * T;
    size_t nrow;
    size_t ncol;
    size_t nrow_alloc; /* To know if we need to extend the size */
    char ** colnames; /* Name of columns can be NULL*/
} ftab_t;

/* Create a new table with a fixed number of columns
 * Set column names with ftab_set_colname */
ftab_t * ftab_new(int ncol);

/* Load a TSV file. The first line is interpreted as
 * containing the column names. Everything else is interpreted
 * as float values.
 */
ftab_t * ftab_from_tsv(const char * fname);

ftab_t * ftab_from_csv(const char * fname);

/* Write tsv file do disk */
int ftab_write_tsv(const ftab_t * T, const char * fname);

/* Write tsv file do disk */
int ftab_write_csv(const ftab_t * T, const char * fname);

/* Write tsv file do disk */
int ftab_write_csv(const ftab_t * T, const char * fname);



/** Print table to file
 * @param[in] fid An open FILE to write to
 * @param[in] T the table to write
 * @param[in] sep The separator or deliminator to use, e.g., "," or "\t"
 * @return EXIT_SUCCESS if file could be written
 */
int ftab_print(FILE * fid, const ftab_t * T, const char * sep);

/* Set the name of a column
* the name is copied, i.e., can be freed */
void ftab_set_colname(ftab_t *, int col, const char * name);

/* Free a ftab and all associated data */
void ftab_free(ftab_t * T);

/* Append a row. Dynamically grows the table if needed. */
void ftab_insert(ftab_t * T, float * row);

/* Get the index of a certain column name
 * Returns -1 on failure. Undefined behavior if
 * multiple columns have the same name. */
int ftab_get_col(const ftab_t * T, const char * name);


/** @brief Set the data for one column.
 * @param T: table to receive data
 * @param col: column to write to
 * @param data: pointer to data to insert
 */

int ftab_set_coldata(ftab_t * T, int col, const float * data);

/** @brief Horizontal concatenation
 *
 * @param L : data on the left side
 * @param R : data on the right side
*/
ftab_t * ftab_concatenate_columns(const ftab_t * L, const ftab_t * R);

/* Keep n head rows */
void ftab_head(ftab_t * T, int64_t n);

/* Run some unit tests */
int ftab_ut(void);

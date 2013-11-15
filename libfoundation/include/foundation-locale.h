/* Copyright (C) 2013 Runtime Revolution Ltd.
 
 This file is part of LiveCode.
 
 LiveCode is free software; you can redistribute it and/or modify it under
 the terms of the GNU General Public License v3 as published by the Free
 Software Foundation.
 
 LiveCode is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 for more details.
 
 You should have received a copy of the GNU General Public License
 along with LiveCode.  If not see <http://www.gnu.org/licenses/>.  */

#ifndef __MC_FOUNDATION_LOCALE__
#define __MC_FOUNDATION_LOCALE__

#include "foundation.h"


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


// Locale text layouts
enum MCLocaleTextLayout
{
    kMCLocaleTextLayoutLTR = 0,     // Left to right
    kMCLocaleTextLayoutRTL = 1,     // Right to left
    kMCLocaleTextLayoutTTB = 2,     // Top to bottom
    kMCLocaleTextLayoutBTT = 3,     // Bottom to top
};

////////////////////////////////////////////////////////////////////////////////

// Opaque pointer type to a locale
typedef struct __MCLocale* MCLocaleRef;

// Gets a reference to the locale that the system is using
bool    MCLocaleCreateDefault(MCLocaleRef &r_locale);

// Gets a reference to the named locale or returns false
bool    MCLocaleCreateWithName(MCStringRef p_locale_name, MCLocaleRef &r_locale);

// Gets a reference to a locale using a Win32 LCID
bool    MCLocaleCreateWithLCID(uint32_t LCID, MCLocaleRef &r_locale);

// Increments the reference count for the given locale
MCLocaleRef MCLocaleRetain(MCLocaleRef p_locale);

// Releases a reference to a locale
void    MCLocaleRelease(MCLocaleRef p_locale);

////////////////////////////////////////////////////////////////////////////////

// Well-known locales. Do not change these directly!
extern MCLocaleRef MCLbasic;        // Compatible with older LiveCode versions
extern MCLocaleRef MCLdefault;      // Locale in use by the system
extern MCLocaleRef MCLcurrent;      // Current locale (if different from default)

////////////////////////////////////////////////////////////////////////////////

// Changes the current locale to the given locale object and updates the rest of
// the engine global variables that depend on the locale (e.g number and date
// formatters).
//
// This releases the locale object held in MCLcurrent and takes ownership of the
// given locale object.
bool MCLocaleSetCurrentLocale(MCLocaleRef);

////////////////////////////////////////////////////////////////////////////////

// For the non-localised variants, the technical, non-display name is returned.
// The stringref belongs to the locale and should not be released.
// For example, the British English locale would return "en" for the language
// and "GB" for the country, corresponding to locale ID "en_GB".
//
// The localised variants return a name suitable for display to the user and
// are in the specified language. The stringref belongs to the caller and needs
// to be released after use. For example, the locale ID "fr_FR" formatted for
// display in "en_GB" would return language "French" and country "France".

// Returns the identifying name of the given locale
MCStringRef MCLocaleGetName(MCLocaleRef p_locale);
bool MCLocaleGetNameLocalised(MCLocaleRef p_locale,
                              MCLocaleRef p_in_language,
                              MCStringRef &r_string);

// Returns the language code of the locale
MCStringRef MCLocaleGetLanguage(MCLocaleRef);
bool MCLocaleGetLanguageLocalised(MCLocaleRef p_locale,
                                  MCLocaleRef p_in_language,
                                  MCStringRef &r_string);

// Returns the country code of the locale
MCStringRef MCLocaleGetCountry(MCLocaleRef);
bool MCLocaleGetCountryLocalised(MCLocaleRef p_locale,
                                 MCLocaleRef p_in_language,
                                 MCStringRef &r_string);

// Returns the writing script used by the locale
MCStringRef MCLocaleGetScript(MCLocaleRef);
bool MCLocaleGetScriptLocalised(MCLocaleRef p_locale,
                                 MCLocaleRef p_in_language,
                                 MCStringRef &r_string);

// Returns the line and character orientation of the locale
MCLocaleTextLayout MCLocaleGetLineDirection(MCLocaleRef);
MCLocaleTextLayout MCLocaleGetCharacterDirection(MCLocaleRef);


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


// Number formatting styles
enum MCNumberFormatStyle
{
    kMCNumberFormatStyleDefault,
    
    kMCNumberFormatStyleDecimalPattern,
    kMCNumberFormatStyleDecimal,
    kMCNumberFormatStyleCurrency,
    kMCNumberFormatStylePercent,
    kMCNumberFormatStyleScientific,
    kMCNumberFormatStyleSpellout,
    kMCNumberFormatStyleOrdinal,
    kMCNumberFormatStyleDuration,
    kMCNumberFormatStyleNumberingSystem,
    kMCNumberFormatStylePatternRulebased,
    kMCNumberFormatStylePatternISO,
    kMCNumberFormatStyleCurrencyISO,
    kMCNumberFormatStyleCurrencyPlural
};

////////////////////////////////////////////////////////////////////////////////

// Opaque number formatter handle
typedef struct __MCNumberFormatter* MCNumberFormatterRef;

// Creates a number formatter for the given number type in the given locale
bool    MCNumberFormatterCreate(MCLocaleRef, MCNumberFormatStyle, MCNumberFormatterRef&);

// Destroys a number formatter object
void    MCNumberFormatterRelease(MCNumberFormatterRef);

////////////////////////////////////////////////////////////////////////////////

// Well-known number formatters using the current locale
extern MCNumberFormatterRef MCNFdecimal;    // Standard decimal number
extern MCNumberFormatterRef MCNFcurrency;   // Currency
extern MCNumberFormatterRef MCNFordinal;    // 1st, 2nd, 3rd, etc

// Well-known number formatters using the "basic" locale
extern MCNumberFormatterRef MCNFbasic;    // Decimal numbers, non-localised

////////////////////////////////////////////////////////////////////////////////

// Converts from an integer or floating-point value to text
bool    MCLocaleNumberFormatInteger(MCNumberFormatterRef, int64_t, MCStringRef&);
bool    MCLocaleNumberFormatReal(MCNumberFormatterRef, real64_t, MCStringRef&);

// Converts from text to an integer or floating-point value
bool    MCLocaleNumberParseInteger(MCNumberFormatterRef, MCStringRef, int64_t&);
bool    MCLocaleNumberParseReal(MCNumberFormatterRef, MCStringRef, real64_t&);


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


// Chooses between a locale's default calendar and the Gregorian calendar
enum MCCalendarType
{
    kMCCalendarTypeDefault,
    kMCCalendarTypeGregorian
};

// Symbolic names for days of the week
enum MCDay
{
    kMCDaySunday = 1,
    kMCDayMonday,
    kMCDayTuesday,
    kMCDayWednesday,
    kMCDayThursday,
    kMCDayFriday,
    kMCDaySaturday
};

// Symbolic names for months of the year
enum MCMonth
{
    kMCMonthJanuary = 0,
    kMCMonthFebruary,
    kMCMonthMarch,
    kMCMonthApril,
    kMCMonthMay,
    kMCMonthJune,
    kMCMonthJuly,
    kMCMonthAugust,
    kMCMonthSeptember,
    kMCMonthOctober,
    kMCMonthNovember,
    kMCMonthDecember,
    kMCMonthUndecimber, // Thirteenth month; required for lunar calendars
};

////////////////////////////////////////////////////////////////////////////////

// Time representation. Count of milliseconds since 1970-01-01 00:00:00
typedef double MCDate;

// NOTE ON TIME ZONES
//
//  A time zone is identified using its TzData name, e.g.
//      America/Los_Angeles
//      Europe/London
//
// TODO: create an opaque Time Zone type
typedef MCStringRef MCTimeZoneRef;

// Opaque type representing a calendar
typedef struct __MCCalendar* MCCalendarRef;

// Creates a calendar object for the given locale and time zone
bool    MCCalendarCreate(MCLocaleRef, MCTimeZoneRef p_time_zone, MCCalendarType, MCCalendarRef&);

// Destroys an existing calendar object
void    MCCalendarRelease(MCCalendarRef);

////////////////////////////////////////////////////////////////////////////////

extern MCTimeZoneRef    MCTZdefault;    // The system time zone
extern MCTimeZoneRef    MCTZutc;        // The UTC time zone

extern MCCalendarRef    MCCdefault;     // The system calendar
extern MCCalendarRef    MCCutc;         // The UTC calendar

////////////////////////////////////////////////////////////////////////////////

// Calculates the difference (in milliseconds) between two times
MCDate  MCDateCompare(MCDate p_first, MCCalendarRef p_first_calendar, MCDate p_second, MCCalendarRef p_second_calendar);

// Converts a date from one calendar to another
MCDate  MCDateConvert(MCDate p_date, MCCalendarRef p_from, MCCalendarRef p_to);

////////////////////////////////////////////////////////////////////////////////

// What is the first day of the week according to the given calendar?
MCDay MCCalendarGetFirstDayOfWeek(MCCalendarRef);

// Returns the time zone associated with the given calendar
MCTimeZoneRef MCCalendarGetTimeZone(MCCalendarRef);

// Is the calendar Gregorian or something else?
bool MCCalendarIsGregorian(MCCalendarRef);


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


// Date format styles
enum MCLocaleDateStyle
{
    kMCLocaleDateStyleDefault   = 0x00,
    kMCLocaleDateStyleFull      = 0x01,
    kMCLocaleDateStyleLong      = 0x02,
    kMCLocaleDateStyleMedium    = 0x03,
    kMCLocaleDateStyleShort     = 0x04,
    kMCLocaleDateStyleNone      = 0x05,
    
    // When set with one of the above options, the date is relative to the
    // current time (e.g. "yesterday", "today" and "tomorrow")
    kMCLocaleDateStyleRelative  = 0x80
};

////////////////////////////////////////////////////////////////////////////////

// Opaque date formatter type
typedef struct __MCDateFormatter* MCDateFormatterRef;

// Creates an object for formatting dates and times using a built-in style
bool    MCLocaleDateFormatterCreateWithStyle(MCCalendarRef p_calendar,
                                             MCLocaleDateStyle p_date_style,
                                             MCLocaleDateStyle p_time_style,
                                             MCDateFormatterRef&);

// Creates an object for formatting dates and times using a pattern. The pattern
// values are those specified by the CLDR for date/time formatting.
bool    MCLocaleDateFormatterCreateWithPattern(MCCalendarRef p_calendar,
                                               MCStringRef p_pattern,
                                               MCDateFormatterRef&);

// Destroys a date formatter object
void    MCLocaleDateFormatterRelease(MCDateFormatterRef);

////////////////////////////////////////////////////////////////////////////////

extern MCDateFormatterRef   MCDFlocal;      // The default local style
extern MCDateFormatterRef   MCDFinternet;   // Internet time
extern MCDateFormatterRef   MCDFdateitems;  // LiveCode dateitems format

////////////////////////////////////////////////////////////////////////////////

// Returns the current time
MCDate MCDateNow();

////////////////////////////////////////////////////////////////////////////////

// Formats a date into a string representation
bool    MCLocaleDateFormat(MCDateFormatterRef, MCDate, MCStringRef&);

// Attempts to parse a date from the supplied format. The parsing may be either
// strict or lenient about formatting mis-matches.
bool    MCLocaleDateParse(MCDateFormatterRef, MCStringRef, bool p_lenient, MCDate&);


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


// Break iterator types
enum MCBreakIteratorType
{
    kMCBreakIteratorTypeCharacter,  // Grapheme boundaries
    kMCBreakIteratorTypeWord,       // Word boundaries
    kMCBreakIteratorTypeLine,       // Line boundaries
    kMCBreakIteratorTypeSentence,   // Sentence boundaries
    kMCBreakIteratorTypeTitle       // Title casing boundaries
};

////////////////////////////////////////////////////////////////////////////////

// Opaque handle to a break iterator
typedef struct __MCBreakIterator* MCBreakIteratorRef;

// Creates a break iterator for the specified locale and type
bool    MCLocaleBreakIteratorCreate(MCLocaleRef, MCBreakIteratorType, MCBreakIteratorRef&);

// Destroys an existing break iterator
void    MCLocaleBreakIteratorRelease(MCBreakIteratorRef);

////////////////////////////////////////////////////////////////////////////////

// Value returned to indicate that there is no more breakable text
#define kMCLocaleBreakIteratorDone  ((uindex_t)-1)

// Sets the text associated with a break iterator
bool    MCLocaleBreakIteratorSetText(MCBreakIteratorRef, MCStringRef);

// Returns the index of the next break point
uindex_t MCLocaleBreakIteratorAdvance(MCBreakIteratorRef);

// Returns whether the given index into the iterator's text is a boundary
bool    MCLocaleBreakIteratorIsBoundary(MCBreakIteratorRef, uindex_t);


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


#endif  /* ifndef __MC_FOUNDATION_LOCALE__ */

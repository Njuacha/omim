/*
  The MIT License (MIT)

  Copyright (c) 2015 Mail.Ru Group

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <type_traits>

namespace osmoh
{

class Time
{
  enum EState
  {
    eIsNotTime = 0,
    eHaveHours = 1,
    eHaveMinutes = 2,
  };

  using TEStateRep = std::underlying_type<EState>::type;

 public:
  enum class EEvent
  {
    eNotEvent,
    eDawn,
    eSunrise,
    eSunset,
    eDusk
  };

  using THours = std::chrono::hours;
  using TMinutes = std::chrono::minutes;

 public:
  Time() = default;
  Time(Time const &) = default;
  Time(THours const hours);
  Time(TMinutes const minutes);
  Time(THours const hours, TMinutes const minutes);
  Time(EEvent const event);

  Time & operator=(Time const &) = default;

  THours::rep GetHoursCount() const;
  TMinutes::rep GetMinutesCount() const;

  THours GetHours() const;
  TMinutes GetMinutes() const;

  void SetHours(THours const hours);
  void SetMinutes(TMinutes const minutes);

  EEvent GetEvent() const {return m_event;}
  void SetEvent(EEvent const event);

  bool IsEvent() const;
  bool IsEventOffset() const;
  bool IsHoursMinutes() const;
  bool IsMinutes() const;
  bool IsTime() const;
  bool HasValue() const;

  Time & operator-(Time const & time);
  Time & operator-();

 private:
  Time GetEventTime() const;

 private:
  EEvent m_event{EEvent::eNotEvent};
  TMinutes m_duration{TMinutes::zero()};
  TEStateRep m_state{EState::eIsNotTime};
};

inline constexpr Time::THours operator ""_h(unsigned long long h)
{
  return Time::THours(h);
}

inline constexpr Time::TMinutes operator ""_min(unsigned long long m)
{
  return Time::TMinutes(m);
}

std::ostream & operator<<(std::ostream & ost, Time::EEvent const event);
std::ostream & operator<<(std::ostream & ost, Time const & time);

class Timespan
{
 public:
  Timespan() = default;
  Timespan(Timespan const &) = default;
  Timespan(Time const & start, bool plus = false);
  Timespan(Time const & start, Time const & end, bool plus = false);
  Timespan(Time const & start, Time const & end, Time const & period);

  bool IsOpen() const;
  bool HasPlus() const;
  bool HasPeriod() const;

  Time const & GetStart() const;
  Time const & GetEnd() const;
  Time const & GetPeriod() const;

  void SetStart(Time const & start);
  void SetEnd(Time const & end);
  void SetPeriod(Time const & period);
  void SetPlus(bool const plus);

  bool IsValid() const;

 private:
  Time m_start;
  Time m_end;
  Time m_period;
  bool m_plus{false};
};

using TTimespans = std::vector<Timespan>;

std::ostream & operator<<(std::ostream & ost, Timespan const & span);
std::ostream & operator<<(std::ostream & ost, osmoh::TTimespans const & timespans);

class NthEntry
{
 public:
  enum class ENth
  {
    None,
    First,
    Second,
    Third,
    Fourth,
    Fifth
  };

 public:
  bool IsEmpty() const;
  bool HasStart() const;
  bool HasEnd() const;

  ENth GetStart() const;
  ENth GetEnd() const;

  void SetStart(ENth const s);
  void SetEnd(ENth const e);

 private:
  ENth m_start{};
  ENth m_end{};
};

std::ostream & operator<<(std::ostream & ost, NthEntry const entry);

enum class EWeekday
{
  None,
  Su,
  Mo,
  Tu,
  We,
  Th,
  Fr,
  Sa
};

inline constexpr EWeekday operator ""_day(unsigned long long day)
{
  using TDay = decltype(day);
  return ((day <= static_cast<TDay>(EWeekday::None) ||
           day > static_cast<TDay>(EWeekday::Sa))
          ? EWeekday::None
          : static_cast<EWeekday>(day));
}

std::ostream & operator<<(std::ostream & ost, EWeekday const wday);

class WeekdayRange
{
  using TNths = std::vector<NthEntry>;

 public:

 public:
  bool HasWday(EWeekday const & wday) const;

  bool HasSu() const;
  bool HasMo() const;
  bool HasTu() const;
  bool HasWe() const;
  bool HasTh() const;
  bool HasFr() const;
  bool HasSa() const;

  bool HasStart() const;
  bool HasEnd() const;
  bool IsEmpty() const;

  EWeekday GetStart() const;
  EWeekday GetEnd() const;
  size_t GetDaysCount() const;

  void SetStart(EWeekday const & wday);
  void SetEnd(EWeekday const & wday);

  int32_t GetOffset() const;
  void SetOffset(int32_t const offset);

  bool HasNth() const;
  TNths const & GetNths() const;

  void AddNth(NthEntry const & entry);

 private:
  EWeekday m_start{};
  EWeekday m_end{};
  int32_t m_offset{};
  TNths m_nths;
};

using TWeekdayRanges = std::vector<WeekdayRange>;

std::ostream & operator<<(std::ostream & ost, WeekdayRange const & range);
std::ostream & operator<<(std::ostream & ost, TWeekdayRanges const & ranges);

class Holiday
{
 public:
  bool IsPlural() const;
  void SetPlural(bool const plural);

  int32_t GetOffset() const;
  void SetOffset(int32_t const offset);

 private:
  bool m_plural{false};
  int32_t m_offset{};
};

using THolidays = std::vector<Holiday>;

std::ostream & operator<<(std::ostream & ost, Holiday const & holiday);
std::ostream & operator<<(std::ostream & ost, THolidays const & holidys);

class Weekdays // Correspond to weekday_selector in osm opening hours
{
 public:
  bool HasWeekday() const;
  bool HasHolidays() const;

  TWeekdayRanges const & GetWeekdayRanges() const;
  THolidays const & GetHolidays() const;

  void SetWeekdayRanges(TWeekdayRanges const ranges);
  void SetHolidays(THolidays const & holidays);

  void AddWeekdayRange(WeekdayRange const range);
  void AddHoliday(Holiday const & holiday);

 private:
  TWeekdayRanges m_weekdayRanges;
  THolidays m_holidays;
};

std::ostream & operator<<(std::ostream & ost, Weekdays const & weekday);

class DateOffset
{
 public:
  bool IsEmpty() const;
  bool HasWDayOffset() const;
  bool HasOffset() const;

  bool IsWDayOffsetPositive() const;

  EWeekday GetWDayOffset() const;
  int32_t GetOffset() const;

  void SetWDayOffset(EWeekday const wday);
  void SetOffset(int32_t const offset);
  void SetWDayOffsetPositive(bool const on);

 private:
  EWeekday m_wday_offset{EWeekday::None};
  bool m_positive{true};
  int32_t m_offset{};
};

std::ostream operator<<(std::ostream & ost, DateOffset const & dateOffset);

class MonthDay
{
 public:
  enum class EMonth
  {
    None,
    Jan,
    Feb,
    Mar,
    Apr,
    May,
    Jun,
    Jul,
    Aug,
    Sep,
    Oct,
    Nov,
    Dec
  };

  enum class EVariableDate
  {
    None,
    Easter
  };

  using TYear = uint8_t;
  using TDayNum = uint8_t;

 public:
  bool IsEmpty() const;
  bool IsVariable() const;

  bool HasYear() const;
  bool HasMonth() const;
  bool HasDayNum() const;
  bool HasOffset() const;

  TYear GetYear() const;
  EMonth GetMonth() const;
  TDayNum GetDayNum() const;
  DateOffset const & GetOffset() const;
  EVariableDate GetVariableDate() const;

  void SetYear(TYear const year);
  void SetMonth(EMonth const month);
  void SetDayNum(TDayNum const daynum);
  void SetOffset(DateOffset const & offset);
  void SetVariableDate(EVariableDate const date);

 private:
  TYear m_year{};
  EMonth m_month{EMonth::None};
  TDayNum m_daynum{};
  EVariableDate m_variable_date{EVariableDate::None};
  DateOffset m_offset{};
};

inline constexpr MonthDay::EMonth operator ""_M(unsigned long long month)
{
  using TMonth = decltype(month);
  return ((month <= static_cast<TMonth>(MonthDay::EMonth::None) ||
           month > static_cast<TMonth>(MonthDay::EMonth::Dec))
          ? MonthDay::EMonth::None
          : static_cast<osmoh::MonthDay::EMonth>(month));
}

std::ostream & operator<<(std::ostream & ost, MonthDay const md);

class MonthdayRange
{
 public:
  bool HasStart() const;
  bool HasEnd() const;
  bool HasPeriod() const;
  bool HasPlus() const;

  MonthDay const & GetStart() const;
  MonthDay const & GetEnd() const;
  uint32_t GetPeriod() const;

  void SetStart(MonthDay const & start);
  void SetEnd(MonthDay const & end);
  void SetPeriod(uint32_t const period);
  void SetPlus(bool const plus);

 private:
  MonthDay m_start;
  MonthDay m_end;
  uint32_t m_period{};
  bool m_plus{false};
};

using TMonthdayRanges = std::vector<MonthdayRange>;

std::ostream operator<<(std::ostream & ost, MonthdayRange const & range);
std::ostream operator<<(std::ostream & ost, TMonthdayRanges const & ranges);
} // namespace osmoh


// class State
// {
//  public:
//   enum EState {
//     eUnknown = 0,
//     eClosed = 1,
//     eOpen = 2
//   };

//   uint8_t state;
//   std::string comment;

//   State() : state(eUnknown) {}
// };

// class TimeRule
// {
//  public:
//   TWeekdays weekdays;
//   TTimeSpans timespan; // TODO(mgsergio) rename to timespans
//   State state;
//   uint8_t int_flags = 0;

//   friend std::ostream & operator <<(std::ostream & s, TimeRule const & r);
// };
// } // namespace osmoh

// class OSMTimeRange
// {
//  public:
//   OSMTimeRange() = default;

//   bool IsValid() const { return m_valid; }
//   bool IsOpen() const { return m_state == osmoh::State::eOpen; }
//   bool IsClosed() const { return m_state == osmoh::State::eClosed; }
//   bool IsUnknown() const { return m_state == osmoh::State::eUnknown; }
//   std::string const & Comment() const { return m_comment; }

//   OSMTimeRange & UpdateState(time_t timestamp);
//   OSMTimeRange & UpdateState(std::string const & timestr,
//                              char const * timefmt="%d-%m-%Y %R");

//   std::string ToString() const;

//   static OSMTimeRange FromString(std::string const & rules);

//  private:
//   bool m_valid{false};
//   osmoh::State::EState m_state{osmoh::State::eUnknown};

//   osmoh::TTimeRules m_rules;
//   std::string m_comment;
// };




/// Trash

// class TimeEx
// {
//  public:
//   enum EFlags
//   {
//     eNone = 0,
//     eHours = 1,
//     eMinutes = 2,
//     ePlus = 4,
//     eMinus = 8,
//     eExt = 16,
//     eSunrise = 32,
//     eSunset = 64
//   };

//   uint8_t hours;
//   uint8_t minutes;
//   uint8_t flags;

//   Time() : hours(0), minutes(0), flags(eNone) {}
//   Time & Hours(uint8_t h) { hours = h; flags |= eHours; return *this; }
//   Time & Minutes(uint8_t m) { minutes = m; flags |= eMinutes; return *this; }
//   Time & Sunset() { flags = eSunset; return *this; }
//   Time & Sunrise() { flags = eSunrise; return *this; }

//   std::string ToString() const;
//   friend std::ostream & operator << (std::ostream & s, Time const & t);
// };

// class TimeSpanEx
// {
//  public:
//   Time from;
//   Time to;
//   uint8_t flags;
//   Time period;

//   TimeSpan() : flags(Time::eNone) {}

//   std::string ToString() const;
//   friend std::ostream & operator << (std::ostream & s, TimeSpan const & span);
// };

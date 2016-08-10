// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.


// First part of user declarations.

#line 37 "cassandra-parser.cpp" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "cassandra-parser.h"

// User implementation prologue.

#line 51 "cassandra-parser.cpp" // lalr1.cc:412
// Unqualified %code blocks.
#line 32 "cassandra.y" // lalr1.cc:413

#include <iterator>
#include <iostream>
#include <algorithm>
#include "cassandra-driver.h"

#line 60 "cassandra-parser.cpp" // lalr1.cc:413


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (/*CONSTCOND*/ false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << std::endl;                  \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


namespace yy {
#line 146 "cassandra-parser.cpp" // lalr1.cc:479

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  CassParser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              // Fall through.
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
  CassParser::CassParser (CassDriver &driver_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      driver (driver_yyarg)
  {}

  CassParser::~CassParser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  inline
  CassParser::by_state::by_state ()
    : state (empty_state)
  {}

  inline
  CassParser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  CassParser::by_state::clear ()
  {
    state = empty_state;
  }

  inline
  void
  CassParser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  inline
  CassParser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  CassParser::symbol_number_type
  CassParser::by_state::type_get () const
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  inline
  CassParser::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  CassParser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
      switch (that.type_get ())
    {
      case 53: // state
      case 54: // paction
      case 55: // obs
        value.move< ElemRef > (that.value);
        break;

      case 23: // "float"
      case 57: // prob
      case 58: // number
        value.move< float > (that.value);
        break;

      case 24: // "int"
      case 32: // value_tail
      case 59: // optional_sign
        value.move< int > (that.value);
        break;

      case 25: // "string"
        value.move< std::string > (that.value);
        break;

      case 34: // state_tail
      case 36: // action_tail
      case 38: // obs_param_tail
      case 56: // ident_list
        value.move< std::vector<std::string> > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

  inline
  CassParser::stack_symbol_type&
  CassParser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
      switch (that.type_get ())
    {
      case 53: // state
      case 54: // paction
      case 55: // obs
        value.copy< ElemRef > (that.value);
        break;

      case 23: // "float"
      case 57: // prob
      case 58: // number
        value.copy< float > (that.value);
        break;

      case 24: // "int"
      case 32: // value_tail
      case 59: // optional_sign
        value.copy< int > (that.value);
        break;

      case 25: // "string"
        value.copy< std::string > (that.value);
        break;

      case 34: // state_tail
      case 36: // action_tail
      case 38: // obs_param_tail
      case 56: // ident_list
        value.copy< std::vector<std::string> > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }


  template <typename Base>
  inline
  void
  CassParser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  CassParser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    switch (yytype)
    {
            case 23: // "float"

#line 80 "cassandra.y" // lalr1.cc:636
        { yyoutput << yysym.value.template as< float > (); }
#line 371 "cassandra-parser.cpp" // lalr1.cc:636
        break;

      case 24: // "int"

#line 80 "cassandra.y" // lalr1.cc:636
        { yyoutput << yysym.value.template as< int > (); }
#line 378 "cassandra-parser.cpp" // lalr1.cc:636
        break;

      case 25: // "string"

#line 80 "cassandra.y" // lalr1.cc:636
        { yyoutput << yysym.value.template as< std::string > (); }
#line 385 "cassandra-parser.cpp" // lalr1.cc:636
        break;

      case 32: // value_tail

#line 80 "cassandra.y" // lalr1.cc:636
        { yyoutput << yysym.value.template as< int > (); }
#line 392 "cassandra-parser.cpp" // lalr1.cc:636
        break;

      case 34: // state_tail

#line 72 "cassandra.y" // lalr1.cc:636
        {
    if (!yysym.value.template as< std::vector<std::string> > ().empty()) {
        std::copy(yysym.value.template as< std::vector<std::string> > ().begin(), yysym.value.template as< std::vector<std::string> > ().end() - 1,
                  std::ostream_iterator<std::string>(yyoutput, ","));
        yyoutput << yysym.value.template as< std::vector<std::string> > ().back();
    }
}
#line 405 "cassandra-parser.cpp" // lalr1.cc:636
        break;

      case 36: // action_tail

#line 72 "cassandra.y" // lalr1.cc:636
        {
    if (!yysym.value.template as< std::vector<std::string> > ().empty()) {
        std::copy(yysym.value.template as< std::vector<std::string> > ().begin(), yysym.value.template as< std::vector<std::string> > ().end() - 1,
                  std::ostream_iterator<std::string>(yyoutput, ","));
        yyoutput << yysym.value.template as< std::vector<std::string> > ().back();
    }
}
#line 418 "cassandra-parser.cpp" // lalr1.cc:636
        break;

      case 38: // obs_param_tail

#line 72 "cassandra.y" // lalr1.cc:636
        {
    if (!yysym.value.template as< std::vector<std::string> > ().empty()) {
        std::copy(yysym.value.template as< std::vector<std::string> > ().begin(), yysym.value.template as< std::vector<std::string> > ().end() - 1,
                  std::ostream_iterator<std::string>(yyoutput, ","));
        yyoutput << yysym.value.template as< std::vector<std::string> > ().back();
    }
}
#line 431 "cassandra-parser.cpp" // lalr1.cc:636
        break;

      case 53: // state

#line 79 "cassandra.y" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ElemRef > ().name; }
#line 438 "cassandra-parser.cpp" // lalr1.cc:636
        break;

      case 54: // paction

#line 79 "cassandra.y" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ElemRef > ().name; }
#line 445 "cassandra-parser.cpp" // lalr1.cc:636
        break;

      case 55: // obs

#line 79 "cassandra.y" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ElemRef > ().name; }
#line 452 "cassandra-parser.cpp" // lalr1.cc:636
        break;

      case 56: // ident_list

#line 72 "cassandra.y" // lalr1.cc:636
        {
    if (!yysym.value.template as< std::vector<std::string> > ().empty()) {
        std::copy(yysym.value.template as< std::vector<std::string> > ().begin(), yysym.value.template as< std::vector<std::string> > ().end() - 1,
                  std::ostream_iterator<std::string>(yyoutput, ","));
        yyoutput << yysym.value.template as< std::vector<std::string> > ().back();
    }
}
#line 465 "cassandra-parser.cpp" // lalr1.cc:636
        break;

      case 57: // prob

#line 80 "cassandra.y" // lalr1.cc:636
        { yyoutput << yysym.value.template as< float > (); }
#line 472 "cassandra-parser.cpp" // lalr1.cc:636
        break;

      case 58: // number

#line 80 "cassandra.y" // lalr1.cc:636
        { yyoutput << yysym.value.template as< float > (); }
#line 479 "cassandra-parser.cpp" // lalr1.cc:636
        break;

      case 59: // optional_sign

#line 80 "cassandra.y" // lalr1.cc:636
        { yyoutput << yysym.value.template as< int > (); }
#line 486 "cassandra-parser.cpp" // lalr1.cc:636
        break;


      default:
        break;
    }
    yyo << ')';
  }
#endif

  inline
  void
  CassParser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  CassParser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  CassParser::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  CassParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  CassParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  CassParser::debug_level_type
  CassParser::debug_level () const
  {
    return yydebug_;
  }

  void
  CassParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  inline CassParser::state_type
  CassParser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  inline bool
  CassParser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  CassParser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  CassParser::parse ()
  {
    // State.
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    // User initialization code.
    #line 25 "cassandra.y" // lalr1.cc:745
{
    yyla.location.begin.filename = yyla.location.end.filename = &driver.file;
}

#line 604 "cassandra-parser.cpp" // lalr1.cc:745

    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            symbol_type yylookahead (yylex (driver));
            yyla.move (yylookahead);
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
        switch (yyr1_[yyn])
    {
      case 53: // state
      case 54: // paction
      case 55: // obs
        yylhs.value.build< ElemRef > ();
        break;

      case 23: // "float"
      case 57: // prob
      case 58: // number
        yylhs.value.build< float > ();
        break;

      case 24: // "int"
      case 32: // value_tail
      case 59: // optional_sign
        yylhs.value.build< int > ();
        break;

      case 25: // "string"
        yylhs.value.build< std::string > ();
        break;

      case 34: // state_tail
      case 36: // action_tail
      case 38: // obs_param_tail
      case 56: // ident_list
        yylhs.value.build< std::vector<std::string> > ();
        break;

      default:
        break;
    }


      // Compute the default @$.
      {
        slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, slice, yylen);
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
      try
        {
          switch (yyn)
            {
  case 10:
#line 100 "cassandra.y" // lalr1.cc:859
    { driver.setDiscount(yystack_[0].value.as< float > ()); }
#line 743 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 11:
#line 103 "cassandra.y" // lalr1.cc:859
    { driver.setWeightSign(yystack_[0].value.as< int > ()); }
#line 749 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 12:
#line 106 "cassandra.y" // lalr1.cc:859
    { yylhs.value.as< int > () = 1; }
#line 755 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 13:
#line 107 "cassandra.y" // lalr1.cc:859
    { yylhs.value.as< int > () = -1; }
#line 761 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 14:
#line 110 "cassandra.y" // lalr1.cc:859
    { driver.setStates(yystack_[0].value.as< std::vector<std::string> > ()); }
#line 767 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 15:
#line 113 "cassandra.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<std::string> > ().resize(yystack_[0].value.as< int > ()); }
#line 773 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 16:
#line 114 "cassandra.y" // lalr1.cc:859
    { std::swap(yylhs.value.as< std::vector<std::string> > (), yystack_[0].value.as< std::vector<std::string> > ()); }
#line 779 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 17:
#line 117 "cassandra.y" // lalr1.cc:859
    { driver.setActions(yystack_[0].value.as< std::vector<std::string> > ()); }
#line 785 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 18:
#line 120 "cassandra.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<std::string> > ().resize(yystack_[0].value.as< int > ()); }
#line 791 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 19:
#line 121 "cassandra.y" // lalr1.cc:859
    { std::swap(yylhs.value.as< std::vector<std::string> > (), yystack_[0].value.as< std::vector<std::string> > ()); }
#line 797 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 20:
#line 124 "cassandra.y" // lalr1.cc:859
    { driver.setObservations(yystack_[0].value.as< std::vector<std::string> > ()); }
#line 803 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 21:
#line 127 "cassandra.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<std::string> > ().resize(yystack_[0].value.as< int > ()); }
#line 809 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 22:
#line 128 "cassandra.y" // lalr1.cc:859
    { std::swap(yylhs.value.as< std::vector<std::string> > (), yystack_[0].value.as< std::vector<std::string> > ()); }
#line 815 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 36:
#line 154 "cassandra.y" // lalr1.cc:859
    {
    driver.addTransition(yystack_[3].value.as< ElemRef > (), yystack_[5].value.as< ElemRef > (), yystack_[1].value.as< ElemRef > (), yystack_[0].value.as< float > ());
}
#line 823 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 37:
#line 157 "cassandra.y" // lalr1.cc:859
    { assert(false); }
#line 829 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 38:
#line 158 "cassandra.y" // lalr1.cc:859
    { assert(false); }
#line 835 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 40:
#line 164 "cassandra.y" // lalr1.cc:859
    {
    driver.addObsTransition(yystack_[3].value.as< ElemRef > (), yystack_[5].value.as< ElemRef > (), yystack_[1].value.as< ElemRef > (), yystack_[0].value.as< float > ());
}
#line 843 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 41:
#line 167 "cassandra.y" // lalr1.cc:859
    { assert(false); }
#line 849 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 42:
#line 168 "cassandra.y" // lalr1.cc:859
    { assert(false); }
#line 855 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 44:
#line 174 "cassandra.y" // lalr1.cc:859
    {
    driver.addWeight(yystack_[5].value.as< ElemRef > (), yystack_[7].value.as< ElemRef > (), yystack_[3].value.as< ElemRef > (), yystack_[1].value.as< ElemRef > (), yystack_[0].value.as< float > ());
}
#line 863 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 45:
#line 177 "cassandra.y" // lalr1.cc:859
    { assert(false); }
#line 869 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 46:
#line 178 "cassandra.y" // lalr1.cc:859
    { assert(false); }
#line 875 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 57:
#line 199 "cassandra.y" // lalr1.cc:859
    { yylhs.value.as< ElemRef > ().type = ELEMREFTYPE_ID; yylhs.value.as< ElemRef > ().id = yystack_[0].value.as< int > (); }
#line 881 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 58:
#line 200 "cassandra.y" // lalr1.cc:859
    { yylhs.value.as< ElemRef > ().type = ELEMREFTYPE_NAME; std::swap(yylhs.value.as< ElemRef > ().name, yystack_[0].value.as< std::string > ()); }
#line 887 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 59:
#line 201 "cassandra.y" // lalr1.cc:859
    { yylhs.value.as< ElemRef > ().type = ELEMREFTYPE_ALL; yylhs.value.as< ElemRef > ().name = "*"; }
#line 893 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 60:
#line 204 "cassandra.y" // lalr1.cc:859
    { yylhs.value.as< ElemRef > ().type = ELEMREFTYPE_ID; yylhs.value.as< ElemRef > ().id = yystack_[0].value.as< int > (); }
#line 899 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 61:
#line 205 "cassandra.y" // lalr1.cc:859
    { yylhs.value.as< ElemRef > ().type = ELEMREFTYPE_NAME; std::swap(yylhs.value.as< ElemRef > ().name, yystack_[0].value.as< std::string > ()); }
#line 905 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 62:
#line 206 "cassandra.y" // lalr1.cc:859
    { yylhs.value.as< ElemRef > ().type = ELEMREFTYPE_ALL; yylhs.value.as< ElemRef > ().name = "*"; }
#line 911 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 63:
#line 209 "cassandra.y" // lalr1.cc:859
    { yylhs.value.as< ElemRef > ().type = ELEMREFTYPE_ID; yylhs.value.as< ElemRef > ().id = yystack_[0].value.as< int > (); }
#line 917 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 64:
#line 210 "cassandra.y" // lalr1.cc:859
    { yylhs.value.as< ElemRef > ().type = ELEMREFTYPE_NAME; std::swap(yylhs.value.as< ElemRef > ().name, yystack_[0].value.as< std::string > ()); }
#line 923 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 65:
#line 211 "cassandra.y" // lalr1.cc:859
    { yylhs.value.as< ElemRef > ().type = ELEMREFTYPE_ALL; yylhs.value.as< ElemRef > ().name = "*"; }
#line 929 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 66:
#line 214 "cassandra.y" // lalr1.cc:859
    { std::swap(yylhs.value.as< std::vector<std::string> > (), yystack_[1].value.as< std::vector<std::string> > ()); yylhs.value.as< std::vector<std::string> > ().push_back(yystack_[0].value.as< std::string > ()); }
#line 935 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 67:
#line 215 "cassandra.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<std::string> > ().push_back(yystack_[0].value.as< std::string > ()); }
#line 941 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 68:
#line 218 "cassandra.y" // lalr1.cc:859
    { yylhs.value.as< float > () = yystack_[0].value.as< int > (); }
#line 947 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 69:
#line 219 "cassandra.y" // lalr1.cc:859
    { yylhs.value.as< float > () = yystack_[0].value.as< float > (); }
#line 953 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 70:
#line 222 "cassandra.y" // lalr1.cc:859
    { yylhs.value.as< float > () = yystack_[1].value.as< int > () * yystack_[0].value.as< int > (); }
#line 959 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 71:
#line 223 "cassandra.y" // lalr1.cc:859
    { yylhs.value.as< float > () = yystack_[1].value.as< int > () * yystack_[0].value.as< float > (); }
#line 965 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 72:
#line 226 "cassandra.y" // lalr1.cc:859
    { yylhs.value.as< int > () = 1; }
#line 971 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 73:
#line 227 "cassandra.y" // lalr1.cc:859
    { yylhs.value.as< int > () = -1; }
#line 977 "cassandra-parser.cpp" // lalr1.cc:859
    break;

  case 74:
#line 228 "cassandra.y" // lalr1.cc:859
    { yylhs.value.as< int > () = 1; }
#line 983 "cassandra-parser.cpp" // lalr1.cc:859
    break;


#line 987 "cassandra-parser.cpp" // lalr1.cc:859
            default:
              break;
            }
        }
      catch (const syntax_error& yyexc)
        {
          error (yyexc);
          YYERROR;
        }
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, yylhs);
    }
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    yyerror_range[1].location = yystack_[yylen - 1].location;
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
  CassParser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
  }

  // Generate an error message.
  std::string
  CassParser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (!yyla.empty ())
      {
        int yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
        YYCASE_(0, YY_("syntax error"));
        YYCASE_(1, YY_("syntax error, unexpected %s"));
        YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char CassParser::yypact_ninf_ = -70;

  const signed char CassParser::yytable_ninf_ = -75;

  const signed char
  CassParser::yypact_[] =
  {
     -70,     5,    53,   -70,   -10,     6,    10,    23,    35,    57,
     -70,   -70,   -70,   -70,   -70,   -70,   -70,    14,    36,    55,
      61,    63,    74,    81,    -8,    54,   -70,   -70,   -70,    66,
     -70,   -70,   -70,   -70,   -70,   -70,    42,   -70,   -70,    42,
     -70,   -70,    42,   -12,   -12,   -70,   -70,   -70,   -70,   -70,
     -70,    68,   -70,    82,    83,    84,   -70,   -70,   -70,   -70,
     -70,   -70,   -70,   -70,   -70,   -70,   -12,   -70,   -12,   -70,
      21,    21,    21,   -70,   -70,   -70,   -70,   -70,    -5,   -70,
       9,   -70,    85,   -70,   -70,   -12,   -70,    68,   -12,   -70,
     -12,    20,    29,    56,   -12,   -70,    41,   -70,   -12,    48,
     -70,    68,   -70,   -70,   -70,    68,    62,   -70,   -70,   -70,
      41,    48,    14,   -70
  };

  const unsigned char
  CassParser::yydefact_[] =
  {
       4,     0,    27,     1,     0,     0,     0,     0,     0,     0,
       3,     5,     6,     7,     8,     9,    31,    74,     0,     0,
       0,     0,     0,     0,     0,     2,    72,    73,    10,     0,
      12,    13,    11,    15,    67,    14,    16,    18,    17,    19,
      21,    20,    22,     0,     0,    50,    51,    69,    68,    24,
      23,    52,    54,     0,     0,     0,    30,    32,    33,    34,
      71,    70,    66,    59,    57,    58,    25,    29,    26,    53,
       0,     0,     0,    28,    62,    60,    61,    35,     0,    39,
       0,    43,     0,    47,    48,     0,    38,    49,     0,    42,
       0,     0,     0,    74,     0,    37,     0,    41,     0,    46,
      56,     0,    65,    63,    64,     0,    74,    55,    36,    40,
       0,    45,    74,    44
  };

  const signed char
  CassParser::yypgoto_[] =
  {
     -70,   -70,   -70,   -70,   -70,   -70,   -70,   -70,   -70,   -70,
     -70,   -70,   -70,   -70,    64,   -70,   -70,   -70,   -70,   -70,
     -70,   -70,   -70,   -70,   -69,    27,     0,   -64,    25,    -3,
      78,   -50,   -17,   -70
  };

  const signed char
  CassParser::yydefgoto_[] =
  {
      -1,     1,     2,    10,    11,    12,    32,    13,    35,    14,
      38,    15,    41,    16,    66,    25,    56,    57,    77,    58,
      79,    59,    81,    86,    50,    51,    99,    67,    78,   105,
      36,    52,   100,    29
  };

  const signed char
  CassParser::yytable_[] =
  {
      28,    69,    73,    45,    73,     3,    83,    84,    63,    17,
      46,    89,    64,    65,    85,    47,    48,    49,    47,    48,
      45,    91,    95,    97,    92,    18,    93,    46,    88,    19,
     101,    45,    47,    48,   106,    26,    27,    69,    46,    94,
      45,    74,    20,    47,    48,    75,    76,    46,    96,    30,
      31,   108,    47,    48,    21,   109,     4,     5,     6,     7,
       8,   102,    53,    54,    55,   103,   104,    62,     9,    26,
      27,   -74,   -74,    22,    23,    98,    24,    26,    27,    33,
      34,   110,   107,    26,    27,    37,    34,    40,    34,    60,
      61,    47,    48,    43,   107,   113,    80,    82,    39,    42,
      44,    70,    71,    72,    90,    87,   111,   112,    68
  };

  const unsigned char
  CassParser::yycheck_[] =
  {
      17,    51,    66,    11,    68,     0,    11,    12,    20,    19,
      18,    80,    24,    25,    19,    23,    24,    25,    23,    24,
      11,    85,    91,    92,    88,    19,    90,    18,    19,    19,
      94,    11,    23,    24,    98,    21,    22,    87,    18,    19,
      11,    20,    19,    23,    24,    24,    25,    18,    19,    13,
      14,   101,    23,    24,    19,   105,     3,     4,     5,     6,
       7,    20,     8,     9,    10,    24,    25,    25,    15,    21,
      22,    23,    24,    16,    17,    19,    19,    21,    22,    24,
      25,    19,    99,    21,    22,    24,    25,    24,    25,    23,
      24,    23,    24,    19,   111,   112,    71,    72,    20,    21,
      19,    19,    19,    19,    19,    78,   106,   110,    44
  };

  const unsigned char
  CassParser::yystos_[] =
  {
       0,    27,    28,     0,     3,     4,     5,     6,     7,    15,
      29,    30,    31,    33,    35,    37,    39,    19,    19,    19,
      19,    19,    16,    17,    19,    41,    21,    22,    58,    59,
      13,    14,    32,    24,    25,    34,    56,    24,    36,    56,
      24,    38,    56,    19,    19,    11,    18,    23,    24,    25,
      50,    51,    57,     8,     9,    10,    42,    43,    45,    47,
      23,    24,    25,    20,    24,    25,    40,    53,    40,    57,
      19,    19,    19,    53,    20,    24,    25,    44,    54,    46,
      54,    48,    54,    11,    12,    19,    49,    51,    19,    50,
      19,    53,    53,    53,    19,    50,    19,    50,    19,    52,
      58,    53,    20,    24,    25,    55,    53,    58,    57,    57,
      19,    52,    55,    58
  };

  const unsigned char
  CassParser::yyr1_[] =
  {
       0,    26,    27,    28,    28,    29,    29,    29,    29,    29,
      30,    31,    32,    32,    33,    34,    34,    35,    36,    36,
      37,    38,    38,    39,    39,    39,    39,    39,    40,    40,
      41,    41,    42,    42,    42,    43,    44,    44,    44,    45,
      46,    46,    46,    47,    48,    48,    48,    49,    49,    49,
      50,    50,    50,    51,    51,    52,    52,    53,    53,    53,
      54,    54,    54,    55,    55,    55,    56,    56,    57,    57,
      58,    58,    59,    59,    59
  };

  const unsigned char
  CassParser::yyr2_[] =
  {
       0,     2,     3,     2,     0,     1,     1,     1,     1,     1,
       3,     3,     1,     1,     3,     1,     1,     3,     1,     1,
       3,     1,     1,     3,     3,     4,     4,     0,     2,     1,
       2,     0,     1,     1,     1,     3,     6,     4,     2,     3,
       6,     4,     2,     3,     8,     6,     4,     1,     1,     1,
       1,     1,     1,     2,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     1,     1,     1,
       2,     2,     1,     1,     0
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const CassParser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "\"discount\"", "\"values\"",
  "\"states\"", "\"actions\"", "\"observations\"", "\"T\"", "\"O\"",
  "\"R\"", "\"uniform\"", "\"identity\"", "\"reward\"", "\"cost\"",
  "\"start\"", "\"include\"", "\"exclude\"", "\"reset\"", "\":\"", "\"*\"",
  "\"+\"", "\"-\"", "\"float\"", "\"int\"", "\"string\"", "$accept",
  "pomdp_file", "preamble", "param_type", "discount_param", "value_param",
  "value_tail", "state_param", "state_tail", "action_param", "action_tail",
  "obs_param", "obs_param_tail", "start_state", "start_state_list",
  "param_list", "param_spec", "trans_prob_spec", "trans_spec_tail",
  "obs_prob_spec", "obs_spec_tail", "reward_spec", "reward_spec_tail",
  "ui_matrix", "u_matrix", "prob_matrix", "num_matrix", "state", "paction",
  "obs", "ident_list", "prob", "number", "optional_sign", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned char
  CassParser::yyrline_[] =
  {
       0,    85,    85,    88,    89,    92,    93,    94,    95,    97,
     100,   103,   106,   107,   110,   113,   114,   117,   120,   121,
     124,   127,   128,   131,   132,   133,   134,   135,   138,   139,
     142,   143,   146,   147,   148,   151,   154,   157,   158,   161,
     164,   167,   168,   171,   174,   177,   178,   181,   182,   183,
     186,   187,   188,   191,   192,   195,   196,   199,   200,   201,
     204,   205,   206,   209,   210,   211,   214,   215,   218,   219,
     222,   223,   226,   227,   228
  };

  // Print the state stack on the debug stream.
  void
  CassParser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  CassParser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG



} // yy
#line 1440 "cassandra-parser.cpp" // lalr1.cc:1167
#line 230 "cassandra.y" // lalr1.cc:1168


void yy::CassParser::error(const location_type &l, const std::string &m) {
    driver.error(l, m);
}

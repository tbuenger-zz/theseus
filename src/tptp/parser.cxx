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
#line 29 "parser.ypp" // lalr1.cc:404
 /*** C/C++ Declarations ***/

#include <stdio.h>
#include <string>
#include <vector>


#line 114 "parser.ypp" // lalr1.cc:404


#include "driver.hpp"
#include "scanner.hpp"

/* this "connects" the bison parser in the driver to the flex scanner class
 * object. it defines the yylex() function call to pull the next token from the
 * current lexer object of the driver context. */
#undef yylex
#define yylex driver.lexer->lex


#line 57 "parser.cxx" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "parser.hxx"

// User implementation prologue.

#line 71 "parser.cxx" // lalr1.cc:412


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

#line 57 "parser.ypp" // lalr1.cc:479
namespace theseus { namespace tptp {
#line 157 "parser.cxx" // lalr1.cc:479

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  Parser::yytnamerr_ (const char *yystr)
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
  Parser::Parser (class Driver& driver_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      driver (driver_yyarg)
  {}

  Parser::~Parser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/

  inline
  Parser::syntax_error::syntax_error (const location_type& l, const std::string& m)
    : std::runtime_error (m)
    , location (l)
  {}

  // basic_symbol.
  template <typename Base>
  inline
  Parser::basic_symbol<Base>::basic_symbol ()
    : value ()
  {}

  template <typename Base>
  inline
  Parser::basic_symbol<Base>::basic_symbol (const basic_symbol& other)
    : Base (other)
    , value ()
    , location (other.location)
  {
      switch (other.type_get ())
    {
      case 23: // problem
      case 24: // definition
      case 26: // formula
      case 27: // negated_formula
      case 28: // quantified_formula
      case 29: // operation_formula
      case 30: // bracketed_formula
      case 31: // predicate_formula
        value.copy< std::shared_ptr<Formula> > (other.value);
        break;

      case 4: // "variable identifier"
      case 5: // "function identifier"
        value.copy< std::string > (other.value);
        break;

      case 33: // variable_list
        value.copy< std::vector<theseus::Identifier*> > (other.value);
        break;

      case 36: // term_list
        value.copy< std::vector<theseus::Term*> > (other.value);
        break;

      case 38: // function
      case 39: // constant
      case 40: // variable
        value.copy< theseus::Identifier* > (other.value);
        break;

      case 34: // operator
        value.copy< theseus::Operation > (other.value);
        break;

      case 32: // quantifier
        value.copy< theseus::Quantifier > (other.value);
        break;

      case 35: // function_term
      case 37: // term
        value.copy< theseus::Term* > (other.value);
        break;

      default:
        break;
    }

  }


  template <typename Base>
  inline
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const semantic_type& v, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {
    (void) v;
      switch (this->type_get ())
    {
      case 23: // problem
      case 24: // definition
      case 26: // formula
      case 27: // negated_formula
      case 28: // quantified_formula
      case 29: // operation_formula
      case 30: // bracketed_formula
      case 31: // predicate_formula
        value.copy< std::shared_ptr<Formula> > (v);
        break;

      case 4: // "variable identifier"
      case 5: // "function identifier"
        value.copy< std::string > (v);
        break;

      case 33: // variable_list
        value.copy< std::vector<theseus::Identifier*> > (v);
        break;

      case 36: // term_list
        value.copy< std::vector<theseus::Term*> > (v);
        break;

      case 38: // function
      case 39: // constant
      case 40: // variable
        value.copy< theseus::Identifier* > (v);
        break;

      case 34: // operator
        value.copy< theseus::Operation > (v);
        break;

      case 32: // quantifier
        value.copy< theseus::Quantifier > (v);
        break;

      case 35: // function_term
      case 37: // term
        value.copy< theseus::Term* > (v);
        break;

      default:
        break;
    }
}


  // Implementation of basic_symbol constructor for each type.

  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::shared_ptr<Formula> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::string v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::vector<theseus::Identifier*> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::vector<theseus::Term*> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const theseus::Identifier* v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const theseus::Operation v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const theseus::Quantifier v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const theseus::Term* v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}


  template <typename Base>
  inline
  Parser::basic_symbol<Base>::~basic_symbol ()
  {
    clear ();
  }

  template <typename Base>
  inline
  void
  Parser::basic_symbol<Base>::clear ()
  {
    // User destructor.
    symbol_number_type yytype = this->type_get ();
    basic_symbol<Base>& yysym = *this;
    (void) yysym;
    switch (yytype)
    {
   default:
      break;
    }

    // Type destructor.
    switch (yytype)
    {
      case 23: // problem
      case 24: // definition
      case 26: // formula
      case 27: // negated_formula
      case 28: // quantified_formula
      case 29: // operation_formula
      case 30: // bracketed_formula
      case 31: // predicate_formula
        value.template destroy< std::shared_ptr<Formula> > ();
        break;

      case 4: // "variable identifier"
      case 5: // "function identifier"
        value.template destroy< std::string > ();
        break;

      case 33: // variable_list
        value.template destroy< std::vector<theseus::Identifier*> > ();
        break;

      case 36: // term_list
        value.template destroy< std::vector<theseus::Term*> > ();
        break;

      case 38: // function
      case 39: // constant
      case 40: // variable
        value.template destroy< theseus::Identifier* > ();
        break;

      case 34: // operator
        value.template destroy< theseus::Operation > ();
        break;

      case 32: // quantifier
        value.template destroy< theseus::Quantifier > ();
        break;

      case 35: // function_term
      case 37: // term
        value.template destroy< theseus::Term* > ();
        break;

      default:
        break;
    }

    Base::clear ();
  }

  template <typename Base>
  inline
  bool
  Parser::basic_symbol<Base>::empty () const
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  inline
  void
  Parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move(s);
      switch (this->type_get ())
    {
      case 23: // problem
      case 24: // definition
      case 26: // formula
      case 27: // negated_formula
      case 28: // quantified_formula
      case 29: // operation_formula
      case 30: // bracketed_formula
      case 31: // predicate_formula
        value.move< std::shared_ptr<Formula> > (s.value);
        break;

      case 4: // "variable identifier"
      case 5: // "function identifier"
        value.move< std::string > (s.value);
        break;

      case 33: // variable_list
        value.move< std::vector<theseus::Identifier*> > (s.value);
        break;

      case 36: // term_list
        value.move< std::vector<theseus::Term*> > (s.value);
        break;

      case 38: // function
      case 39: // constant
      case 40: // variable
        value.move< theseus::Identifier* > (s.value);
        break;

      case 34: // operator
        value.move< theseus::Operation > (s.value);
        break;

      case 32: // quantifier
        value.move< theseus::Quantifier > (s.value);
        break;

      case 35: // function_term
      case 37: // term
        value.move< theseus::Term* > (s.value);
        break;

      default:
        break;
    }

    location = s.location;
  }

  // by_type.
  inline
  Parser::by_type::by_type ()
    : type (empty_symbol)
  {}

  inline
  Parser::by_type::by_type (const by_type& other)
    : type (other.type)
  {}

  inline
  Parser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
  Parser::by_type::clear ()
  {
    type = empty_symbol;
  }

  inline
  void
  Parser::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  inline
  int
  Parser::by_type::type_get () const
  {
    return type;
  }
  // Implementation of make_symbol for each symbol type.
  Parser::symbol_type
  Parser::make_END (const location_type& l)
  {
    return symbol_type (token::END, l);
  }

  Parser::symbol_type
  Parser::make_EOL (const location_type& l)
  {
    return symbol_type (token::EOL, l);
  }

  Parser::symbol_type
  Parser::make_VAR_LABEL (const std::string& v, const location_type& l)
  {
    return symbol_type (token::VAR_LABEL, v, l);
  }

  Parser::symbol_type
  Parser::make_FUNC_LABEL (const std::string& v, const location_type& l)
  {
    return symbol_type (token::FUNC_LABEL, v, l);
  }

  Parser::symbol_type
  Parser::make_LEFT (const location_type& l)
  {
    return symbol_type (token::LEFT, l);
  }

  Parser::symbol_type
  Parser::make_RIGHT (const location_type& l)
  {
    return symbol_type (token::RIGHT, l);
  }

  Parser::symbol_type
  Parser::make_LEFT_SQUARE (const location_type& l)
  {
    return symbol_type (token::LEFT_SQUARE, l);
  }

  Parser::symbol_type
  Parser::make_RIGHT_SQUARE (const location_type& l)
  {
    return symbol_type (token::RIGHT_SQUARE, l);
  }

  Parser::symbol_type
  Parser::make_AND (const location_type& l)
  {
    return symbol_type (token::AND, l);
  }

  Parser::symbol_type
  Parser::make_OR (const location_type& l)
  {
    return symbol_type (token::OR, l);
  }

  Parser::symbol_type
  Parser::make_ARROW (const location_type& l)
  {
    return symbol_type (token::ARROW, l);
  }

  Parser::symbol_type
  Parser::make_ALL (const location_type& l)
  {
    return symbol_type (token::ALL, l);
  }

  Parser::symbol_type
  Parser::make_SOME (const location_type& l)
  {
    return symbol_type (token::SOME, l);
  }

  Parser::symbol_type
  Parser::make_AXIOM (const location_type& l)
  {
    return symbol_type (token::AXIOM, l);
  }

  Parser::symbol_type
  Parser::make_CONJECTURE (const location_type& l)
  {
    return symbol_type (token::CONJECTURE, l);
  }

  Parser::symbol_type
  Parser::make_FOF (const location_type& l)
  {
    return symbol_type (token::FOF, l);
  }

  Parser::symbol_type
  Parser::make_NEGATION (const location_type& l)
  {
    return symbol_type (token::NEGATION, l);
  }



  // by_state.
  inline
  Parser::by_state::by_state ()
    : state (empty_state)
  {}

  inline
  Parser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  Parser::by_state::clear ()
  {
    state = empty_state;
  }

  inline
  void
  Parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  inline
  Parser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  Parser::symbol_number_type
  Parser::by_state::type_get () const
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  inline
  Parser::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  Parser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
      switch (that.type_get ())
    {
      case 23: // problem
      case 24: // definition
      case 26: // formula
      case 27: // negated_formula
      case 28: // quantified_formula
      case 29: // operation_formula
      case 30: // bracketed_formula
      case 31: // predicate_formula
        value.move< std::shared_ptr<Formula> > (that.value);
        break;

      case 4: // "variable identifier"
      case 5: // "function identifier"
        value.move< std::string > (that.value);
        break;

      case 33: // variable_list
        value.move< std::vector<theseus::Identifier*> > (that.value);
        break;

      case 36: // term_list
        value.move< std::vector<theseus::Term*> > (that.value);
        break;

      case 38: // function
      case 39: // constant
      case 40: // variable
        value.move< theseus::Identifier* > (that.value);
        break;

      case 34: // operator
        value.move< theseus::Operation > (that.value);
        break;

      case 32: // quantifier
        value.move< theseus::Quantifier > (that.value);
        break;

      case 35: // function_term
      case 37: // term
        value.move< theseus::Term* > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

  inline
  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
      switch (that.type_get ())
    {
      case 23: // problem
      case 24: // definition
      case 26: // formula
      case 27: // negated_formula
      case 28: // quantified_formula
      case 29: // operation_formula
      case 30: // bracketed_formula
      case 31: // predicate_formula
        value.copy< std::shared_ptr<Formula> > (that.value);
        break;

      case 4: // "variable identifier"
      case 5: // "function identifier"
        value.copy< std::string > (that.value);
        break;

      case 33: // variable_list
        value.copy< std::vector<theseus::Identifier*> > (that.value);
        break;

      case 36: // term_list
        value.copy< std::vector<theseus::Term*> > (that.value);
        break;

      case 38: // function
      case 39: // constant
      case 40: // variable
        value.copy< theseus::Identifier* > (that.value);
        break;

      case 34: // operator
        value.copy< theseus::Operation > (that.value);
        break;

      case 32: // quantifier
        value.copy< theseus::Quantifier > (that.value);
        break;

      case 35: // function_term
      case 37: // term
        value.copy< theseus::Term* > (that.value);
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
  Parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  Parser::yy_print_ (std::ostream& yyo,
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
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  inline
  void
  Parser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  Parser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  Parser::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Parser::debug_level_type
  Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  inline Parser::state_type
  Parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  inline bool
  Parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  Parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Parser::parse ()
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
    #line 68 "parser.ypp" // lalr1.cc:745
{
    // initialize the initial location object
    yyla.location.begin.filename = yyla.location.end.filename = &driver.streamname;
}

#line 995 "parser.cxx" // lalr1.cc:745

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
            yyla.type = yytranslate_ (yylex (&yyla.value, &yyla.location));
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
      case 23: // problem
      case 24: // definition
      case 26: // formula
      case 27: // negated_formula
      case 28: // quantified_formula
      case 29: // operation_formula
      case 30: // bracketed_formula
      case 31: // predicate_formula
        yylhs.value.build< std::shared_ptr<Formula> > ();
        break;

      case 4: // "variable identifier"
      case 5: // "function identifier"
        yylhs.value.build< std::string > ();
        break;

      case 33: // variable_list
        yylhs.value.build< std::vector<theseus::Identifier*> > ();
        break;

      case 36: // term_list
        yylhs.value.build< std::vector<theseus::Term*> > ();
        break;

      case 38: // function
      case 39: // constant
      case 40: // variable
        yylhs.value.build< theseus::Identifier* > ();
        break;

      case 34: // operator
        yylhs.value.build< theseus::Operation > ();
        break;

      case 32: // quantifier
        yylhs.value.build< theseus::Quantifier > ();
        break;

      case 35: // function_term
      case 37: // term
        yylhs.value.build< theseus::Term* > ();
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
  case 2:
#line 129 "parser.ypp" // lalr1.cc:859
    { driver.expressions.emplace_back(std::move(yystack_[0].value.as< std::shared_ptr<Formula> > ())); }
#line 1147 "parser.cxx" // lalr1.cc:859
    break;

  case 3:
#line 130 "parser.ypp" // lalr1.cc:859
    { driver.expressions.emplace_back(std::move(yystack_[0].value.as< std::shared_ptr<Formula> > ())); }
#line 1153 "parser.cxx" // lalr1.cc:859
    break;

  case 4:
#line 135 "parser.ypp" // lalr1.cc:859
    {yylhs.value.as< std::shared_ptr<Formula> > () = std::move(yystack_[2].value.as< std::shared_ptr<Formula> > ());}
#line 1159 "parser.cxx" // lalr1.cc:859
    break;

  case 7:
#line 142 "parser.ypp" // lalr1.cc:859
    {yylhs.value.as< std::shared_ptr<Formula> > () = std::move(yystack_[0].value.as< std::shared_ptr<Formula> > ());}
#line 1165 "parser.cxx" // lalr1.cc:859
    break;

  case 8:
#line 143 "parser.ypp" // lalr1.cc:859
    {yylhs.value.as< std::shared_ptr<Formula> > () = std::move(yystack_[0].value.as< std::shared_ptr<Formula> > ());}
#line 1171 "parser.cxx" // lalr1.cc:859
    break;

  case 9:
#line 144 "parser.ypp" // lalr1.cc:859
    {yylhs.value.as< std::shared_ptr<Formula> > () = std::move(yystack_[0].value.as< std::shared_ptr<Formula> > ());}
#line 1177 "parser.cxx" // lalr1.cc:859
    break;

  case 10:
#line 148 "parser.ypp" // lalr1.cc:859
    {yylhs.value.as< std::shared_ptr<Formula> > () = std::make_shared<theseus::FormulaNegation>(std::move(yystack_[0].value.as< std::shared_ptr<Formula> > ()));}
#line 1183 "parser.cxx" // lalr1.cc:859
    break;

  case 11:
#line 152 "parser.ypp" // lalr1.cc:859
    {yylhs.value.as< std::shared_ptr<Formula> > () = std::make_shared<theseus::FormulaQuantifier>(yystack_[5].value.as< theseus::Quantifier > (), std::move(yystack_[3].value.as< std::vector<theseus::Identifier*> > ()), std::move(yystack_[0].value.as< std::shared_ptr<Formula> > ()));}
#line 1189 "parser.cxx" // lalr1.cc:859
    break;

  case 12:
#line 156 "parser.ypp" // lalr1.cc:859
    {yylhs.value.as< std::shared_ptr<Formula> > () = std::make_shared<theseus::FormulaJunction>(yystack_[1].value.as< theseus::Operation > (), std::move(pairtolist(std::move(yystack_[2].value.as< std::shared_ptr<Formula> > ()), std::move(yystack_[0].value.as< std::shared_ptr<Formula> > ()))));}
#line 1195 "parser.cxx" // lalr1.cc:859
    break;

  case 13:
#line 157 "parser.ypp" // lalr1.cc:859
    {yylhs.value.as< std::shared_ptr<Formula> > () = std::move(yystack_[0].value.as< std::shared_ptr<Formula> > ());}
#line 1201 "parser.cxx" // lalr1.cc:859
    break;

  case 14:
#line 159 "parser.ypp" // lalr1.cc:859
    {yylhs.value.as< std::shared_ptr<Formula> > () = std::make_shared<theseus::FormulaJunction>(yystack_[1].value.as< theseus::Operation > (), std::move(pairtolist(std::move(yystack_[2].value.as< std::shared_ptr<Formula> > ()), std::move(yystack_[0].value.as< std::shared_ptr<Formula> > ()))));}
#line 1207 "parser.cxx" // lalr1.cc:859
    break;

  case 15:
#line 160 "parser.ypp" // lalr1.cc:859
    {yylhs.value.as< std::shared_ptr<Formula> > () = std::move(yystack_[0].value.as< std::shared_ptr<Formula> > ());}
#line 1213 "parser.cxx" // lalr1.cc:859
    break;

  case 16:
#line 163 "parser.ypp" // lalr1.cc:859
    {yylhs.value.as< std::shared_ptr<Formula> > () = std::move(yystack_[1].value.as< std::shared_ptr<Formula> > ());}
#line 1219 "parser.cxx" // lalr1.cc:859
    break;

  case 17:
#line 166 "parser.ypp" // lalr1.cc:859
    {yylhs.value.as< std::shared_ptr<Formula> > () = std::make_shared<theseus::FormulaLiteral>(Literal(yystack_[0].value.as< theseus::Term* > ()));}
#line 1225 "parser.cxx" // lalr1.cc:859
    break;

  case 18:
#line 169 "parser.ypp" // lalr1.cc:859
    { yylhs.value.as< theseus::Quantifier > () = theseus::Quantifier::Universal;}
#line 1231 "parser.cxx" // lalr1.cc:859
    break;

  case 19:
#line 170 "parser.ypp" // lalr1.cc:859
    { yylhs.value.as< theseus::Quantifier > () = theseus::Quantifier::Existential;}
#line 1237 "parser.cxx" // lalr1.cc:859
    break;

  case 20:
#line 173 "parser.ypp" // lalr1.cc:859
    {yylhs.value.as< std::vector<theseus::Identifier*> > () = enlist(yystack_[2].value.as< std::vector<theseus::Identifier*> > (), std::move(yystack_[0].value.as< theseus::Identifier* > ()));}
#line 1243 "parser.cxx" // lalr1.cc:859
    break;

  case 21:
#line 174 "parser.ypp" // lalr1.cc:859
    {yylhs.value.as< std::vector<theseus::Identifier*> > () = {1, yystack_[0].value.as< theseus::Identifier* > ()};}
#line 1249 "parser.cxx" // lalr1.cc:859
    break;

  case 22:
#line 177 "parser.ypp" // lalr1.cc:859
    { yylhs.value.as< theseus::Operation > () = theseus::Operation::Disjunction;}
#line 1255 "parser.cxx" // lalr1.cc:859
    break;

  case 23:
#line 178 "parser.ypp" // lalr1.cc:859
    { yylhs.value.as< theseus::Operation > () = theseus::Operation::Conjunction;}
#line 1261 "parser.cxx" // lalr1.cc:859
    break;

  case 24:
#line 179 "parser.ypp" // lalr1.cc:859
    { yylhs.value.as< theseus::Operation > () = theseus::Operation::Conjunction;}
#line 1267 "parser.cxx" // lalr1.cc:859
    break;

  case 25:
#line 184 "parser.ypp" // lalr1.cc:859
    { yylhs.value.as< theseus::Term* > () = theseus::TermDatabase::instance().get(yystack_[3].value.as< theseus::Identifier* > (), yystack_[1].value.as< std::vector<theseus::Term*> > ()); }
#line 1273 "parser.cxx" // lalr1.cc:859
    break;

  case 26:
#line 187 "parser.ypp" // lalr1.cc:859
    {yylhs.value.as< std::vector<theseus::Term*> > () = enlist(yystack_[2].value.as< std::vector<theseus::Term*> > (), std::move(yystack_[0].value.as< theseus::Term* > ()));}
#line 1279 "parser.cxx" // lalr1.cc:859
    break;

  case 27:
#line 188 "parser.ypp" // lalr1.cc:859
    {yylhs.value.as< std::vector<theseus::Term*> > () = {1, yystack_[0].value.as< theseus::Term* > ()};}
#line 1285 "parser.cxx" // lalr1.cc:859
    break;

  case 28:
#line 191 "parser.ypp" // lalr1.cc:859
    { yylhs.value.as< theseus::Term* > () = theseus::TermDatabase::instance().get(yystack_[0].value.as< theseus::Identifier* > ()); }
#line 1291 "parser.cxx" // lalr1.cc:859
    break;

  case 29:
#line 192 "parser.ypp" // lalr1.cc:859
    { yylhs.value.as< theseus::Term* > () = yystack_[0].value.as< theseus::Term* > (); }
#line 1297 "parser.cxx" // lalr1.cc:859
    break;

  case 30:
#line 193 "parser.ypp" // lalr1.cc:859
    { yylhs.value.as< theseus::Term* > () = theseus::TermDatabase::instance().get(yystack_[0].value.as< theseus::Identifier* > ()); }
#line 1303 "parser.cxx" // lalr1.cc:859
    break;

  case 31:
#line 197 "parser.ypp" // lalr1.cc:859
    {yylhs.value.as< theseus::Identifier* > () = theseus::IdentifierFactory::instance().function(yystack_[0].value.as< std::string > ());}
#line 1309 "parser.cxx" // lalr1.cc:859
    break;

  case 32:
#line 200 "parser.ypp" // lalr1.cc:859
    {yylhs.value.as< theseus::Identifier* > () = yystack_[0].value.as< theseus::Identifier* > ();}
#line 1315 "parser.cxx" // lalr1.cc:859
    break;

  case 33:
#line 203 "parser.ypp" // lalr1.cc:859
    {yylhs.value.as< theseus::Identifier* > () = theseus::IdentifierFactory::instance().variable(yystack_[0].value.as< std::string > ());}
#line 1321 "parser.cxx" // lalr1.cc:859
    break;


#line 1325 "parser.cxx" // lalr1.cc:859
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
  Parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
  }

  // Generate an error message.
  std::string
  Parser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
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


  const signed char Parser::yypact_ninf_ = -35;

  const signed char Parser::yytable_ninf_ = -1;

  const signed char
  Parser::yypact_[] =
  {
      -5,     8,     1,   -35,    20,   -35,   -35,   -35,   -35,    13,
      18,    15,     2,   -35,     2,   -35,   -35,     2,    28,   -35,
     -35,   -35,    -1,    -1,    29,   -35,    30,    31,   -35,    19,
     -35,   -35,   -35,    21,    21,    37,    24,   -35,   -35,   -35,
     -35,   -35,     4,   -35,   -35,    -2,   -35,    30,   -35,   -35,
      22,    37,   -35,    24,     2,   -35,   -35,   -35
  };

  const unsigned char
  Parser::yydefact_[] =
  {
       0,     0,     0,     3,     0,     1,     2,     6,     5,     0,
       0,     0,     0,    31,     0,    18,    19,     0,     0,     9,
       7,     8,    13,    15,     0,    17,     0,     0,    10,     0,
      23,    22,    24,     0,     0,     0,     0,    16,     4,    12,
      14,    33,     0,    21,    29,     0,    27,    32,    30,    28,
       0,     0,    25,     0,     0,    20,    26,    11
  };

  const signed char
  Parser::yypgoto_[] =
  {
     -35,   -35,    40,   -35,   -14,   -35,   -35,    -3,   -35,   -35,
     -35,   -35,    23,   -34,   -35,    -9,   -32,   -35,   -29
  };

  const signed char
  Parser::yydefgoto_[] =
  {
      -1,     2,     3,     9,    18,    19,    20,    21,    22,    23,
      24,    42,    33,    25,    45,    46,    26,    48,    49
  };

  const unsigned char
  Parser::yytable_[] =
  {
      27,     5,    44,    28,    47,    52,    43,    13,    14,    30,
      31,    32,     1,    50,     4,    15,    16,    53,     1,    44,
      17,    47,    55,    51,     7,     8,    13,    14,    41,    13,
      39,    40,    10,    11,    12,    29,    36,    35,    37,    38,
      57,    41,     6,    54,    56,     0,    34
  };

  const signed char
  Parser::yycheck_[] =
  {
      14,     0,    36,    17,    36,     7,    35,     5,     6,    10,
      11,    12,    17,     9,     6,    13,    14,    19,    17,    53,
      18,    53,    51,    19,     4,     5,     5,     6,     4,     5,
      33,    34,    19,    15,    19,     7,     6,     8,     7,    20,
      54,     4,     2,    21,    53,    -1,    23
  };

  const unsigned char
  Parser::yystos_[] =
  {
       0,    17,    23,    24,     6,     0,    24,     4,     5,    25,
      19,    15,    19,     5,     6,    13,    14,    18,    26,    27,
      28,    29,    30,    31,    32,    35,    38,    26,    26,     7,
      10,    11,    12,    34,    34,     8,     6,     7,    20,    29,
      29,     4,    33,    40,    35,    36,    37,    38,    39,    40,
       9,    19,     7,    19,    21,    40,    37,    26
  };

  const unsigned char
  Parser::yyr1_[] =
  {
       0,    22,    23,    23,    24,    25,    25,    26,    26,    26,
      27,    28,    29,    29,    29,    29,    30,    31,    32,    32,
      33,    33,    34,    34,    34,    35,    36,    36,    37,    37,
      37,    38,    39,    40
  };

  const unsigned char
  Parser::yyr2_[] =
  {
       0,     2,     2,     1,     9,     1,     1,     1,     1,     1,
       2,     6,     3,     1,     3,     1,     3,     1,     1,     1,
       3,     1,     1,     1,     1,     4,     3,     1,     1,     1,
       1,     1,     1,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const Parser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "\"end of line\"",
  "\"variable identifier\"", "\"function identifier\"", "LEFT", "RIGHT",
  "LEFT_SQUARE", "RIGHT_SQUARE", "AND", "OR", "ARROW", "ALL", "SOME",
  "AXIOM", "CONJECTURE", "FOF", "NEGATION", "','", "'.'", "':'", "$accept",
  "problem", "definition", "description", "formula", "negated_formula",
  "quantified_formula", "operation_formula", "bracketed_formula",
  "predicate_formula", "quantifier", "variable_list", "operator",
  "function_term", "term_list", "term", "function", "constant", "variable", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned char
  Parser::yyrline_[] =
  {
       0,   129,   129,   130,   135,   138,   139,   142,   143,   144,
     147,   151,   155,   157,   158,   160,   163,   166,   169,   170,
     173,   174,   177,   178,   179,   183,   187,   188,   191,   192,
     193,   197,   200,   203
  };

  // Print the state stack on the debug stream.
  void
  Parser::yystack_print_ ()
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
  Parser::yy_reduce_print_ (int yyrule)
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

  // Symbol number corresponding to token number t.
  inline
  Parser::token_number_type
  Parser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
     0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    19,     2,    20,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    21,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18
    };
    const unsigned int user_token_number_max_ = 273;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }

#line 57 "parser.ypp" // lalr1.cc:1167
} } // theseus::tptp
#line 1776 "parser.cxx" // lalr1.cc:1167
#line 207 "parser.ypp" // lalr1.cc:1168
 /*** Additional Code ***/

void theseus::tptp::Parser::error(const Parser::location_type& l,
                const std::string& m)
{
    driver.error(l, m);
}
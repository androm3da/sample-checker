
#include <memory>
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugReporter.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/CheckerRegistry.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CallEvent.h"

#include "llvm/Support/raw_ostream.h"

using namespace clang;
using namespace clang::ento;
using namespace llvm;

namespace {

    class FloatExcessPrecisionChecker: public Checker < check::Bind,
                                                        check::PostStmt<FloatingLiteral>
                                                        > 
    {
        mutable std::unique_ptr<BugType> bugType;

        public:
        FloatExcessPrecisionChecker() 
			: bugType(new BugType("Excess floating-point precision", "androm3da analyzer"))
        {
        }
        
        void checkPostStmt(const FloatingLiteral *fl, CheckerContext &C) const;
        void checkBind(SVal 	Loc,
                       SVal 	Val,
                       const Stmt *Statement,
                       CheckerContext &ctxt
                       )		const;
    };

} 

void FloatExcessPrecisionChecker::checkBind(SVal 	Loc,
                       SVal 	Val,
                       const Stmt *Statement,
                       CheckerContext &ctxt
                       )		const
{
    const bool foundbug = false;

    if (foundbug)
    {
        ExplodedNode  *loc = ctxt.generateSink();

        BugReport *bug = new BugReport(*this->bugType, "all bugs all the time", loc);
        ctxt.emitReport(bug);
    }
}

#include <iostream>

void FloatExcessPrecisionChecker::checkPostStmt(const FloatingLiteral *fl, CheckerContext &ctxt) const
{

#if 0
    const bool is_perf_critical = std::any_of(parents.begin(),
            parents.end(),
            [](const ast_type_traits::DynTypedNode parent) { return parent.get<Stmt>.getName() == "PerfCritical"; } );
#endif
    auto parents = ctxt.getASTContext().getParents(*fl);
    const bool is_intentional = !parents.empty() 
        && parents[0].get<CXXStaticCastExpr>() != nullptr 
        && parents[0].get<CXXStaticCastExpr>()->getTypeAsWritten().getAsString() == "double";

    const bool is_double =  &fl->getSemantics() == &APFloat::IEEEdouble;
    const bool foundbug = !is_intentional && is_double;

    if (foundbug)
    {
        ExplodedNode  *loc = ctxt.generateSink();

        BugReport *bug = new BugReport(*this->bugType, 
                "literal w/double precision, may impact performance", loc);
        ctxt.emitReport(bug);
    }
}


extern "C"
const char clang_analyzerAPIVersionString[] = CLANG_ANALYZER_API_VERSION_STRING;

extern "C"
void clang_registerCheckers(CheckerRegistry &registry) {
  registry.addChecker<FloatExcessPrecisionChecker>("performance.crit.ExcessPrecision", "float");
}

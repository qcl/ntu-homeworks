Program				:	BlockStmt
					;
BlockStmt			:	"{"	VarDecl	Stmt	"}"
					;
VarDecl				:	int	VarDeclNum	id	VarDeclId	";"	VarDecl
					|
					;
VarDeclNum			:	"["	num	"]"	VarDeclNum
					|
					;
VarDeclId			:	","	id	VarDeclId
					|
					;
Var					:	id	VarArithExpr
					;
VarArithExpr		:	"["	ArithExpr	"]" VarArithExpr
					|
					;
Stmt				:	Var	"="	ArithExpr	";"	Stmt
					|	"if"	"("	LogicExpr	")"	BlockStmt	StmtElse	Stmt
					|	"while"	"("	LogicExpr	")"	BlockStmt	Stmt
					|	"break"	";"	Stmt
					|	"continue"	";"	Stmt
					|	"scan"	"("	Var	StmtVar	")"	";"	Stmt
					|	"print"	"("	PrintableExpr	StmtPrintableExpr	")"	";"	Stmt
					|	BlockStmt	Stmt
					|
					;
StmtElse			:	"else"	BlockStmt
					|
					;
StmtVar				:	","	Var	StmtVar
					|
					;
StmtPrintableExpr	:	","	PrintableExpr	StmtPrintableExpr
					|
					;
PrintableExpr		:	string
					|	ArithExpr
					;
ArithExpr			:	ArithExprMD	ArithExprASMore
					;
ArithExprMD			:	ArithExprPN	ArithExprMDMore
					;
ArithExprASMore		:	"+"	ArithExprMD	ArithExprASMore
					|	"-"	ArithExprMD	ArithExprASMore
					|
					;
ArithExprPN			:	"+"	ArithExprBase
					|	"-" ArithExprBase
					|	ArithExprBase
					;
ArithExprBase		:	num
					|	Var
					|	"("	ArithExpr	")"
					;
ArithExprMDMore		:	"*"	ArithExprPN	ArithExprMDMore
					|	"/"	ArithExprPN	ArithExprMDMore
					|
					;
LogicExpr			:	LogicExprAnd	LogicExprOrMore
					;
LogicExprAnd		:	LogicExprTF	LogicExprAndMore
                	;
LogicExprOrMore		:	"||"	LogicExprAnd	LogicExprOrMore
					|
					;
LogicExprTF			:	"!"	LogicExprBase
					|	LogicExprBase
					;
LogicExprBase		:	ArithExpr	LogicExprComp
					|	"["	LogicExpr	"]"
					;
LogicExprAndMore	:	"&&"	LogicExprTF	LogicExprAndMore
					|
					;
LogicExprComp		:	">"		ArithExpr
					|	">="	ArithExpr
					|	"<"		ArithExpr
					|	"<="	ArithExpr
					|	"=="	ArithExpr
					|	"!="	ArithExpr
					;

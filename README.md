# CPSC323-Parser-SyntaxAnalysis

Grammar Rules: 
1.	S -> A
2.	A -> id =  E ;
3.	E -> TE'
4.	E' -> + TE' | - TE' |  e
5.	T -> FT'
6.	T' -> * FT' | / FT' |  e
7.	F -> ( E ) | id

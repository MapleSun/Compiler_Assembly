##Rat 16S Rules
----
*Remove left recursive and backtrack*


R1. \<Rat16S>  ::=  \<Opt Function Definitions>  
$$  \<Opt Declaration List>   
$$  \<Statement List>  
$$

R2. \<Opt Function Definitions> ::= \<Function Definitions> | \<Empty>   
R3. \<Function Definitions> ::= \<Function> \<FDPrime>   
R4. \<FDPrime>  ::= \<Empty> | \<Function Definitions>   

R5. \<Function> ::= function  \<Identifier>  ( \<Opt Parameter List> )  \<Opt Declaration List>  \<Body>   
R6. \<Opt Parameter List> ::=  \<Parameter List>   |  \<Empty>   
R7. \<Parameter List> ::= \<Parameter> \<PLPrime>   
R8. \<PLPrime>  ::= \<Empty> | , \<Parameter List>   

R9. \<Parameter> ::=  \<IDs > : \<Qualifier>    
R10. \<Qualifier> ::= integer   |  boolean  |  real   
R11. \<Body>  ::=  {  \< Statement List>  }   
R12. \<Opt Declaration List> ::= \<Declaration List>   | \<Empty>   
R13. \<Declaration List> ::= \<Declaration>;\<DLPrime>   
R14. \<DLPrime> ::= \<Empty> | \<Declaration List>   


R15. \<Declaration> ::=  \<Qualifier > \<IDs>                     
R16. \<IDs> ::= \<Identifier>\<IDsPrime>   
R17. \<IDsPrime> ::= \<Empty> | , \<IDsPrime>   


R18. \<Statement List> ::= \<Statement>\<SLPrime>   
R19. \<SLPrime> ::= \<Empty> | \<Statement List>   

R20. \<Statement> ::=  \<Compound> | \<Assign> | \<If> |  \<Return> | \<Write> | \<Read> | \<While>    
R21. \<Compound> ::= {  \<Statement List>  }    
R22. \<Assign> ::=   \<Identifier> := \<Expression> ;   
R23. \<If> ::= if ( \<Condition>) \<Statement> \<IfPrime>   
R24. \<IfPrime> ::= endif | else \<Statement> endif   

R25. \<Return> ::= return \<ReturnPrime>   
R26. \<ReturnPrime> ::= ; | \<Expression>;   

R27. \<Write> ::=   printf ( \<Expression>);   
R28. \<Read> ::=    scanf ( \<IDs> );   
R29. \<While> ::= while ( \<Condition>  )  \<Statement>    
R30. \<Condition> ::= \<Expression>  \<Relop>   \<Expression>   
R31. \<Relop> ::=   = |  /=  |   >   | \<   |  =>   | \<=              
R32. \<Expression> ::= \<Term>\<ExpressionPrime>   
R33. \<ExpressionPrime> ::= +\<Term>\<ExpressionPrime> | -\<Term>\<ExpressionPrime> | \<Empty>   

R34. \<Term> ::= \<Factor>\<TermPrime>   
R35. \<TermPrime>  ::= *\<Factor>\<TermPrime> | /\<Factor>\<TermPrime> | \<Empty>   


R36. \<Factor> ::= - \<Primary>   | \<Primary>   
R37. \<Primary> ::= \<Identifier> \<PrimaryPrime>| \<Integer> |    ( \<Expression> ) |  \<Real>  | true | false   
R38. \<PrimaryPrime> ::= Empty | \<IDs>   
R39. \<Empty>   ::= empty   





Simplified Rat 16S:

```
* The Simplified Rat16S has NO \<Function definitions>
* No "real" type is allowed
```


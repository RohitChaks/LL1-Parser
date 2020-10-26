# LL1-Parser
A top-down parser that uses a one-token lookahead is called an LL(1) parser.
The first L indicates that the input is read from left to right.
The second L says that it produces a left-to-right derivation.
And the 1 says that it uses one lookahead token. (Some parsers look ahead at the next 2 tokens, or even more than that.)


The following repo contains a C code which takes the number of productions and a regular grammar as input and generates the FirstPos , and FollowPos of the non - Terminals and displays its corresponding LL(1) Parsing Table. Then it will ask the user for a sample string to demonstrate its LL(1) parsing action. Each step of the LL(1) parsing is then shown with the final result at the end :- Either the string gets accepted or rejected by our LL(1) parser !! The test cases have been provided above for the user's reference(make sure you follow the format as shown in the sample test cases).

Following are some of the assumptions I have made :-

Epsilon is represented by '#'.

Productions are of the form A=B , where 'A' is a single Non-Terminal and 'B' can be any combination of Terminals and Non- Terminals.

The L.H.S. of the first production rule is the start symbol.

Grammer is not left recursive.

Each production of a non terminal is entered on a different line.

Only Upper Case letters are Non-Terminals and everything else is a terminal.

Do not use '!' or '$' as they are reserved for special purposes.

All input Strings have to end with a '$'.


#Note: 
I'm sorry for this not-so-great documentation, I had built it as a part of my college's course work.
If you face problems, Please reach out to me, I'd love to help you out. 

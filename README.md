Storage=Global
### Recursive Descent Parser
#### Austin Hester
##### CS 4280 - P3

For static semantics, I am implementing global and local scopes.

I am implementing option #3, that is an FSA table + table.

	The FSA table is convienently located is fsatable.c
	The FSA driver is located in fsadriver.c


A parser, preprocessor, and lexical analyzer for a programming language.

I did not pay too much attention to the language itself. 
This project was to build a parse tree from a ```FILE*```.

```main.c``` first filters the source file using ```filter.c``` 
and ```trimline.c```, leaving out comments
and characters not in the alphabet. This creates a 
new ```wordlist_t*```. It then displays the filtered source.  

Next, a ```token_t**``` is initialized and filled in using the 
```parser``` function. This function goes through our filtered 
word list, appending new tokens to our token list. The parser then pops the first token from the head of the list and begins building a parse tree.

The driver of this program is ```fsadriver.c```. It takes a 
```wordlist_t*``` and returns the next token, keeping track of 
its location in the word list. ```fsadriver``` is quite complex, 
but it is loosely based off of the psuedocode in our notes.

The driver identifies states and tokens using FSA table located 
in ```fsatable.c```. This table is essentially switch and if 
statements.

https://github.com/ahester57/lexical-analyzer
https://github.com/ahester57/Recursive-Descent

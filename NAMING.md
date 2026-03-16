# Conventions

A first attempt at putting down the Naming Conventions to be used in the project. 
Rules listed in no specific order. 

## Naming Rules


### Camel Case, not Pascal Case
Though Pascal seems to be the general rule of thumb for Unreal projects, I will not be using it. It is dumb and I will not elaborate further. This does however bring the advantage of easily differentiating between Engine functionality and project functionality. 

### No b in front of every boolean 
This just feels like a cheap way to avoid having to keep your code self-documenting. 

## Coding Rules 

### Smart Pointers = Good Pointers
Use the unreal engine smart pointers like TObjectPtr or TWeakPtr
Also forgetting about this one a lot.

### (Almost) Always (const) Auto (reference) 
Readability, maintainability, performance. This rule hits all of 'em. 

### Order of private, protected and public
Maintain the order in every class header. 

## Sources
Inspirations to be added here

The Github app decided to delete everything, so more will be added later. 
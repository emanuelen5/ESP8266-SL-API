# XML Parser for SL app
## Functional description
### XML parser
The XML parser takes a string and returns the start and ending position of a tag.

#### Tests
  
  * Can find a node within a string

Two offline example files are used for development tests: **Station lookup** and **Departure lookup**. For both, test that:

  * `StatusCode == 0` (OK)
  * ResponseData node exists

Write a function that checks that a *node* exists and has a specific text value (for leaves only).
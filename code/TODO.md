## Goals

#### Store Event context
struct UserInterfaceContext
{
  int mousex;
  int mousey;
  int mousedown;
}
Update this in the input polling loop
Feed it into the Inventory system
### Click handlers



### Generate IDs for each grid cell and element

Hover over (x,y) -> (X,Y)
Set up a cross-hair over that index
  - Item selected
Give each box an id.
Based on hovered-over (X, Y), compute selected box.
Make a transparent overlay over each object: Initially just another color.
For selected element, draw the transparent element.
  - Selected Item

### Think of library usage

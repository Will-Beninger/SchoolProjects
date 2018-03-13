//**************************************************************************//
// CPSC4660  Fall 2013     Prof: Wendy Osborn                               //
// File: MyPoint.java                                                       //
// Purpose: Stores two x/y points                                           //
// Author: William Beninger 001159518 & Krysti Bouttell-Bonnar 001148083    //
// Date: November 25, 2013                                                  //
//**************************************************************************//

public class MyPoint
{
    //variables
    private Object x;
    private Object y;
    
    //Constructor
    public MyPoint()
    {
      x=0;
      y=0;
    }
    
    public MyPoint(Object f, Object s)
    {
      x=f;
      y=s;
    }
    
    public MyPoint(MyPoint i)
    {
      x=i.getx();
      y=i.gety();
    }
    
    //Accessors
    public Object getx() {return x;}
    public Object gety() {return y;}
    
    //Mutators
    public void setx(Object i) {x=i;}
    public void sety(Object i) {y=i;}
} //Close Class MyPoint
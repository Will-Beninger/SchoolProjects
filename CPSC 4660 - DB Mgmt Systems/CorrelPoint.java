//**************************************************************************//
// CPSC4660  Fall 2013     Prof: Wendy Osborn                               //
// File: MyPoint.java                                                       //
// Purpose: Stores 4 Correllogram Values                                    //
// Author: William Beninger 001159518 & Krysti Bouttell-Bonnar 001148083    //
// Date: November 25, 2013                                                  //
//**************************************************************************//

public class CorrelPoint
{
    //variables
    private double a;
    private double b;
    private double c;
    private double d;
    
    //Constructor
    public CorrelPoint(double a1, double b1, double c1, double d1)
    {
      a=a1;
      b=b1;
      c=c1;
      d=d1;
    }
    
    public CorrelPoint(CorrelPoint i)
    {
      a=i.geta();
      b=i.getb();
      c=i.getc();
      d=i.getd();
    }
    
    //Accessors
    public double geta() {return a;}
    public double getb() {return b;}
    public double getc() {return c;}
    public double getd() {return d;}
    
    //Mutators
    public void seta(double x) {a=x;}
    public void setb(double x) {b=x;}
    public void setc(double x) {c=x;}
    public void setd(double x) {d=x;}
} //Close Class MyPoint
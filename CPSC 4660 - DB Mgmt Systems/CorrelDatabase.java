//**************************************************************************//
// CPSC4660  Fall 2013     Prof: Wendy Osborn                               //
// File: CorrelDatabase.java                                                //
// Purpose: Database to store all of the Correl Database                    //
// Author: William Beninger 001159518 & Krysti Bouttell-Bonnar 001148083    //
// Date: November 25, 2013                                                  //
//**************************************************************************//

import java.util.Vector;
import java.io.File;

public class CorrelDatabase
{
  //Variables
  private File filename;
  private CorrelVector correlVect;
  
  //Constant
  private static int NUMCOLORS = 64;
  
  public CorrelDatabase(File file1, CorrelVector c1)
  {
    filename = file1;
    correlVect = c1;
  }
  
  public CorrelDatabase(String flatfile)
  {
    String firstDelims = "[~]";
    String[] firstTokens = flatfile.split(firstDelims);
    for (int j=0; j<firstTokens.length; j++)
    filename = new File(firstTokens[0]);
    
    String secondString = firstTokens[1];
    String secondDelims = "[,]";
    String[] secondTokens = secondString.split(secondDelims);
    
    CorrelPoint[] arrayOfColors = new CorrelPoint[NUMCOLORS];
    for(int i=0; i<NUMCOLORS; i++)
    {
    String thirdString = secondTokens[i];
    String thirdDelims = "[/]";
    String[] thirdTokens = thirdString.split(thirdDelims);
    arrayOfColors[i] = new CorrelPoint(Double.parseDouble(thirdTokens[0]), 
                                       Double.parseDouble(thirdTokens[1]), 
                                       Double.parseDouble(thirdTokens[2]),
                                       Double.parseDouble(thirdTokens[3]));
    }
    correlVect = new CorrelVector(arrayOfColors);
  }
  
  public File getFilename() {return filename;}
  public CorrelVector getCorrel() {return correlVect;}
  
  public String printDB()
  {
    String output = filename.toString() + "~";
    
    for (int i=0; i<correlVect.size(); i++)
    {
      output += correlVect.get(i).geta() + "/" + correlVect.get(i).getb() + "/" 
        + correlVect.get(i).getc() + "/" + correlVect.get(i).getd();
      if (i!=(correlVect.size()-1))
      {
        output += ",";
      }
    }
    
    return output;
  }
}
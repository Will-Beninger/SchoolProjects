//**************************************************************************//
// CPSC4660  Fall 2013     Prof: Wendy Osborn                               //
// File: HistogramDatabase.java                                             //
// Purpose: Database to store all of the Histogram vectors                  //
// Author: William Beninger 001159518 & Krysti Bouttell-Bonnar 001148083    //
// Date: November 25, 2013                                                  //
//**************************************************************************//

import java.util.Vector;
import java.io.File;

public class HistogramDatabase
{
  //Variables
  private File filename;
  private HistogramVector histVect;
  
  //Constant
  private static int NUMCOLORS = 64;
  
  public HistogramDatabase(File file1, HistogramVector h1)
  {
    filename = file1;
    histVect = h1;
  }
  
  public HistogramDatabase(String flatfile)
  {
    String firstDelims = "[~]";
    String[] firstTokens = flatfile.split(firstDelims);
    for (int j=0; j<firstTokens.length; j++)
    filename = new File(firstTokens[0]);
    
    String secondString = firstTokens[1];
    String secondDelims = "[,]";
    String[] secondTokens = secondString.split(secondDelims);
    
    int[] arrayOfColors = new int[NUMCOLORS];
    for(int i=0; i<NUMCOLORS; i++)
    {
    arrayOfColors[i] = (Integer.parseInt(secondTokens[i]));
    }
    histVect = new HistogramVector(arrayOfColors);
  }
  
  public File getFilename() {return filename;}
  public HistogramVector getHist() {return histVect;}
  
  public String printDB()
  {
    String output = filename.toString() + "~";
    output += printHist();
    return output;
  }
  
  private String printHist()
  {
    String output = "";
    
    for (int i=0; i<histVect.size(); i++)
    {
      output += histVect.get(i);
      if (i!=(histVect.size()-1))
      {
        output += ",";
      }
    }
    return output;
  }
}
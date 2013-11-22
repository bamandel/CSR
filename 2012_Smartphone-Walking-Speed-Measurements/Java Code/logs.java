import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Scanner;

public class logs {

	/**
	 * @param args
	 * @throws IOException 
	 */
	public static void main(String[] args) throws IOException{
		// TODO Auto-generated method stub
		
		FileWriter outFilex=new FileWriter(args[1]);
		PrintWriter outx=new PrintWriter(outFilex);
		FileWriter outFiley=new FileWriter(args[2]);
		PrintWriter outy=new PrintWriter(outFiley);
		FileWriter outFilez=new FileWriter(args[3]);
		PrintWriter outz=new PrintWriter(outFilez);
		FileWriter outFilev=new FileWriter(args[4]);
		PrintWriter outv=new PrintWriter(outFilev);
		FileWriter outFilepitch=new FileWriter(args[5]);
		PrintWriter outpitch=new PrintWriter(outFilepitch);
		FileWriter outFileyaw=new FileWriter(args[6]);
		PrintWriter outyaw=new PrintWriter(outFileyaw);
		FileWriter outFileroll=new FileWriter(args[7]);
		PrintWriter outroll=new PrintWriter(outFileroll);
		FileWriter outFilehMag=new FileWriter(args[8]);
		PrintWriter outhMag=new PrintWriter(outFilehMag);
		Scanner log1=new Scanner(new File(args[0]));
		
		
		//Axes are arbitrary, therefore x, y, z while three vectors, are essentially projected onto the 2-D horizontal plane
		ArrayList<Double> x=new ArrayList<Double>(); //A list of all x-horizontal accelerations measured from the accelerometer
		ArrayList<Double> y=new ArrayList<Double>(); //A list of all y-horizontal accelerations measured from the accelerometer
		ArrayList<Double> z=new ArrayList<Double>();//A list of all z-horizontal accelerations measured from the accelerometer
		ArrayList<Double> v=new ArrayList<Double>();//A list of all vertical accelerations, calculated on phone
		ArrayList<Double> h=new ArrayList<Double>();//A list of all horizontal accelerations, calculated on phone
		ArrayList<Double> yaw=new ArrayList<Double>();//A list of all values of the yaw angle, measured from gyroscope data
		
		ArrayList<Double> time=new ArrayList<Double>(); //A list to store the current millisecond value of each measurement
		int counter=0;
		double curTimeChange=0;
		double xVel=0;
		double yVel=0;
		double zVel=0;
		double vVel=0; //this is the vertical velocity. Currently unused, mostly to double check that it ended up close to zero at the end of the program
		double yawval=0; //this angle, 
		time.add((double) 0);
		
		while (log1.hasNextLine()){  //this loop essentially parses the console.log file, and records the data in it to many files
			//data is also recorded with a timestamp, as calculated at the end of this loop
			String d=log1.nextLine();
			Scanner log=new Scanner(d);
			log.next();
			
			String q=log.next();
			time.add(Double.parseDouble(q.substring(q.length()-3, q.length())));
			
			log.next();
			
			String firstword=log.next();
			if(firstword.equals("New")) continue; //eventual plan: essentially cause keyword "New" to result in a different walking speed test
			
			outx.print(curTimeChange+", ");
			xVel=Double.parseDouble(log.next());
			outx.println(xVel);
			x.add(xVel);
			log.next();
			outy.print(curTimeChange+", ");
			yVel=Double.parseDouble(log.next());
			outy.println(yVel);
			y.add(yVel);
			log.next();
			outz.print(curTimeChange+", ");
			zVel=Double.parseDouble(log.next());
			outz.println(zVel);
			z.add(zVel);
			log.next();
			outv.print(curTimeChange+", ");
			vVel=Double.parseDouble(log.next());
			outv.println(vVel);
			v.add(vVel);

			log.next();
			outroll.print(curTimeChange+",");
			outroll.println(log.next());
			log.next();
			outpitch.print(curTimeChange+",");
			outpitch.println(log.next());
			log.next();
			yawval=Double.parseDouble(log.next());
			yaw.add(yawval);
			outyaw.print(curTimeChange+",");
			outyaw.println(yawval);
			
			log.next();
			log.next(); //this was formerly the place that was marked if a step had occurred, however, this functionality was removed
			counter++;
			
			outhMag.print(curTimeChange+",");
			outhMag.println(Math.sqrt(xVel*xVel+yVel*yVel+zVel*zVel));
			h.add(Math.sqrt(xVel*xVel+yVel*yVel+zVel*zVel));
			curTimeChange+=timeDifference(time.get(counter-1), time.get(counter));
		}
		int walkingCounter=0; //this counter starts when the threshold is reached and walking is assumed to have started
		
		walkingCounter=0;
		int preWalkCounter=3; //set at 3 to make sure there is no null pointer error
		
		while (z.get(walkingCounter)<.1||walkingCounter<20){ //if sufficient z, assume walking has started.
			walkingCounter++;
		}
		
		walkingCounter-=5;  //the start of the stride that triggers the threshold usually starts at least 5 points before
		
		int timeCounter=walkingCounter;
		
		double totaltime=0;
		while(timeCounter<z.size()-10){ //adds up total time from when walking is started to when data stopped recording
			totaltime+=timeDifference(time.get(timeCounter-1), time.get(timeCounter));
			timeCounter++;
		}
		while(preWalkCounter<walkingCounter){ //used to establish starting velocities, uses simple trapezoidal rule for integration
			curTimeChange=timeDifference(time.get(preWalkCounter-1), time.get(preWalkCounter));
			
			zVel+=integrateTrap(curTimeChange, z.get(preWalkCounter-1), z.get(preWalkCounter));
			preWalkCounter++;
		}
		
		double avgz=0;
		double adjustment=0;
		double zacceltot=0;
		int counter88=walkingCounter;
		while (counter88<z.size()){
			zacceltot+=z.get(counter88)*timeDifference(time.get(counter88-1), time.get(counter88));
			counter88++;
		}
		
		System.out.println("Total zaccel: "+zacceltot);
		double estVel=1.50;
		adjustment=adjustmentApprox(zacceltot, totaltime, estVel);
		System.out.println(adjustment);
		int feedbackCounter=0;
		avgz=performIntegration(totaltime, adjustment, zVel, walkingCounter, time, z);
		while (Math.abs(avgz-estVel)>0.13&&feedbackCounter<3&&avgz<2.2){
			System.out.println("Est Val: "+estVel);
			estVel+=((avgz-estVel)/2);
			
			System.out.println("Z was: "+avgz);
			adjustment=adjustmentApprox(zacceltot, totaltime, estVel);
			avgz=performIntegration(totaltime, adjustment, zVel, walkingCounter, time, z);
			feedbackCounter++;
		}
		System.out.println("Estimated speed was:" +estVel);
		
		
		
		double actualspeed=0;
		if (estVel*totaltime<90){
		actualspeed=66/(totaltime); //this is the number of meters traveled divided by the total time-must be changed if distance changes
		}else actualspeed=132/(totaltime);
		System.out.println("Actual Speed was: "+actualspeed);
		System.out.println("Avgz was: "+avgz);
		System.out.println("Percent difference was: "+(Math.abs(avgz-actualspeed)/actualspeed)*100);
		System.out.println("Estimated Distance traversed was: " +avgz*totaltime);
		
	}
	public static double timeDifference(double time1, double time2){
		if (time2>time1){
			return (time2-time1)/1000;
		}else return (time2+1000-time1)/1000;
	}
	public static double integrateSimpson(double time, double a, double b, double c){
		return ((9.8*time*(a+4*b+c))/6);
	}
	public static double integrateTrap(double time, double a, double b){
		return 4.9*time*(a+b);
	}
	public static double performIntegration(double ttime, double adjustment, double zStartVal, int startpoint, ArrayList<Double> time, ArrayList<Double> z){
		int mainCounter=startpoint;
		double zVel=zStartVal;
		double timeChange=0;
		double timeDrift=1;
		double overallZ=0;
		while (mainCounter<time.size()-10){
			timeChange=timeDifference(time.get(mainCounter-1), time.get(mainCounter+1));
			zVel+=integrateSimpson(timeChange, z.get(mainCounter-1), z.get(mainCounter), z.get(mainCounter+1));
			if (z.get(mainCounter)<0.1&&z.get(mainCounter)>-0.1&&z.get(mainCounter-1)<0.1&&z.get(mainCounter-1)>-0.1
					&&z.get(mainCounter-2)<0.1&&z.get(mainCounter-2)>-0.1&&z.get(mainCounter-3)<0.1&&z.get(mainCounter-3)>-0.1&&z.get(mainCounter-4)<0.1&&z.get(mainCounter-4)>-0.1
					&&z.get(mainCounter-5)<0.1&&z.get(mainCounter-5)>-0.1&&z.get(mainCounter-6)<0.1&&z.get(mainCounter-6)>-0.1&&z.get(mainCounter-7)<0.1&&z.get(mainCounter-7)>-0.1
					){
				zVel*=0.9;
			}
			if (z.get(mainCounter)<0){
				zVel*=(.984-adjustment-Math.log(timeDrift)/100);
				}
			overallZ+=(zVel*timeChange);
			mainCounter+=2;
			timeDrift+=(0.01*timeDrift/100);
		}
		return Math.abs(overallZ)/ttime;
	}
	public static double adjustmentApprox(double zacceltot, double totaltime, double speedestimate){
		double distapprox=(totaltime*speedestimate)/66;
		
		if (distapprox<1) distapprox=1;  //assumes that no one is walking less than 60 meters, was found to be inaccurate for smaller distances
		zacceltot*=58/distapprox;
		
		double adjustment=0;
		System.out.println(zacceltot);
		if (Math.abs(zacceltot)>700){
			adjustment=(0.006)/250*(Math.pow((-(zacceltot)-120), 1.2));
		}
		else if (Math.abs(zacceltot)>400){
			
			adjustment=(0.0075)/250*(Math.pow((-(zacceltot)-120), 1.2));
		}
		else if (Math.abs(zacceltot)>330){
			adjustment=(0.008)/250*(Math.pow((-(zacceltot)-120), 1.2));
		}
		
		else if (Math.abs(zacceltot)>240){
			adjustment=(0.011)/250*(Math.pow((-(zacceltot)-120), 1.2));
		}
		else if (Math.abs(zacceltot)>150){
		adjustment=(0.015)/250*(Math.pow(-(zacceltot)-120, 1.2));
		//adjustment=(0.009)/120*(-zacceltot-110);
		}else if (Math.abs(zacceltot)>120){
			adjustment=(0.002)/250*(Math.pow(-(zacceltot)-120, 1.2));
		}
		else adjustment=-0.0013;
		
		return adjustment;
	}
}

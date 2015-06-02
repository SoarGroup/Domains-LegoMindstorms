package edu.umich.eecs.soar.lego;

import java.io.IOException;

import edu.umich.eecs.soar.lego.core.LegoCore;

public class LegoWorld
{
	private static final String AgentsFolder = "./TutorialAgents/";
	private static final String MapsFolder = "./maps/";
	private static final String SoarDebuggerJarPath = System.getenv("SOAR_HOME") + "/SoarJavaDebugger.jar";
	private static String AgentFile = "Advanced-Stage4-completed";
	private static String Map = "AdvancedTutorialTestMap";
	private static Integer seed = new Integer((int)(Math.random() * 10000.0));
	private static Integer TimeDelayMiliseconds = 0;
	
	public static void main(String[] args) throws IOException
	{
		if (args.length >= 1)
			Map = args[0];
		
		if (args.length >= 2)
			AgentFile = args[1];
		
		if (args.length >= 3)
			seed = new Integer(args[2]);
		
		if (args.length >= 4)
			TimeDelayMiliseconds = new Integer(args[3]).intValue();
		
		LegoCore lego = new LegoCore();
		lego.run(TimeDelayMiliseconds, SoarDebuggerJarPath, AgentFile, Map, seed, AgentsFolder, MapsFolder);
	}
}

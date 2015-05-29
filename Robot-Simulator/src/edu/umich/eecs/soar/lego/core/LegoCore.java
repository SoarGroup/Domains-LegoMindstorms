package edu.umich.eecs.soar.lego.core;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JTextField;
import javax.swing.SpringLayout;

import sml.Agent;
import sml.Agent.RunEventInterface;
import sml.Kernel;
import sml.smlRunEventId;

public class LegoCore implements ActionListener, RunEventInterface
{
	LegoMap		map;
	World		world;
	Kernel		kernel;
	Agent		agent;
	
	JFrame		resetWindow;
	
	JLabel		agentFileLabel					= new JLabel("Agent File: ");
	JLabel		mapNameLabel					= new JLabel("Map Name: ");
	JLabel		seedLabel						= new JLabel("Seed: ");
	JLabel		timeDelayLabel					= new JLabel("Time Delay (ms): ");
	
	JTextField	agentFileTextField				= new JTextField("", 20);
	JTextField	mapNameTextField				= new JTextField("", 20);
	JTextField	seedTextField					= new JTextField("", 20);
	JTextField	timeDelayTextField				= new JTextField("", 20);
	JButton		resetButton						= new JButton("Reset");
	JButton		runSoarButton					= new JButton("Run Soar");
	JButton		stopSoarButton					= new JButton("Stop Soar");
	
	boolean		stopSoar						= false;
	boolean		resetSoar						= false;
	boolean		soarRunning						= false;
	
	String agentFolder;
	String mapFolder;
	String soarJavaDebuggerPath;
	
	class ResetSoar implements Runnable
	{
		LegoCore	legoCore;
		
		ResetSoar(LegoCore core)
		{
			legoCore = core;
		}
		
		@Override
		public void run()
		{
			while (resetSoar)
				;
			
			kernel.Shutdown();
			world.d.frame.setVisible(false);
			legoCore.startWorld();
		}
	}
	
	ResetSoar	resetSoarRunnable	= new ResetSoar(this);
	
	public LegoCore()
	{
		resetWindow = new JFrame("Lego World Settings");
		SpringLayout layout = new SpringLayout();
		resetWindow.setLayout(layout);
		resetWindow.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		resetWindow.setResizable(false);
		
		resetWindow.getContentPane().add(agentFileLabel);
		resetWindow.getContentPane().add(mapNameLabel);
		resetWindow.getContentPane().add(seedLabel);
		resetWindow.getContentPane().add(timeDelayLabel);
		
		resetWindow.getContentPane().add(agentFileTextField);
		resetWindow.getContentPane().add(mapNameTextField);
		resetWindow.getContentPane().add(seedTextField);
		resetWindow.getContentPane().add(timeDelayTextField);
		resetWindow.getContentPane().add(resetButton);
		resetWindow.getContentPane().add(runSoarButton);
		resetWindow.getContentPane().add(stopSoarButton);
		
		layout.putConstraint(SpringLayout.WEST, agentFileLabel, 48, SpringLayout.WEST, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.WEST, mapNameLabel, 45, SpringLayout.WEST, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.WEST, seedLabel, 82, SpringLayout.WEST, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.WEST, timeDelayLabel, 10, SpringLayout.WEST, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.WEST, runSoarButton, 95, SpringLayout.WEST, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.WEST, stopSoarButton, 190, SpringLayout.WEST, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.WEST, resetButton, 150, SpringLayout.WEST, resetWindow.getContentPane());
		
		layout.putConstraint(SpringLayout.NORTH, agentFileLabel, 10, SpringLayout.NORTH, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.NORTH, mapNameLabel, 40, SpringLayout.NORTH, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.NORTH, seedLabel, 70, SpringLayout.NORTH, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.NORTH, timeDelayLabel, 100, SpringLayout.NORTH, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.NORTH, runSoarButton, 130, SpringLayout.NORTH, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.NORTH, stopSoarButton, 130, SpringLayout.NORTH, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.NORTH, resetButton, 160, SpringLayout.NORTH, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.SOUTH, resetWindow.getContentPane(), 5, SpringLayout.SOUTH, resetButton);
		
		layout.putConstraint(SpringLayout.WEST, agentFileTextField, 5, SpringLayout.EAST, agentFileLabel);
		layout.putConstraint(SpringLayout.WEST, mapNameTextField, 5, SpringLayout.EAST, mapNameLabel);
		layout.putConstraint(SpringLayout.WEST, seedTextField, 5, SpringLayout.EAST, seedLabel);
		layout.putConstraint(SpringLayout.WEST, timeDelayTextField, 5, SpringLayout.EAST, timeDelayLabel);
		
		layout.putConstraint(SpringLayout.NORTH, agentFileTextField, 5, SpringLayout.NORTH, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.NORTH, mapNameTextField, 35, SpringLayout.NORTH, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.NORTH, seedTextField, 65, SpringLayout.NORTH, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.NORTH, timeDelayTextField, 95, SpringLayout.NORTH, resetWindow.getContentPane());
		
		layout.putConstraint(SpringLayout.EAST, resetWindow.getContentPane(), 5, SpringLayout.EAST, agentFileTextField);
		
		resetButton.addActionListener(this);
		runSoarButton.addActionListener(this);
		stopSoarButton.addActionListener(this);
		
		resetWindow.pack();
		resetWindow.setVisible(true);
	}
	
	public void run(Integer TimeDelayMiliseconds, String SoarDebuggerJarPath, String AgentFile, String Map, Integer seed, String agentsFolder, String mapsFolder)
	{
		timeDelayTextField.setText(TimeDelayMiliseconds.toString());
		agentFileTextField.setText(AgentFile);
		mapNameTextField.setText(Map);
		seedTextField.setText(seed.toString());
		
		agentFolder = agentsFolder;
		mapFolder = mapsFolder;
		
		startWorld();
	}
	
	private void startWorld()
	{
		map = new LegoMap(mapFolder + mapNameTextField.getText());
		
		world = new World(map, new Integer(timeDelayTextField.getText()));
		
		kernel = Kernel.CreateKernelInNewThread(14242);
		agent = kernel.CreateAgent("Lego Soar");
		
		System.out.println("Using Seed: " + seedTextField.getText());
		agent.ExecuteCommandLine("srand " + seedTextField.getText());
		agent.RegisterForRunEvent(smlRunEventId.smlEVENT_BEFORE_INPUT_PHASE, this, null);
		
		long runEventIdentifier = agent.RegisterForRunEvent(smlRunEventId.smlEVENT_BEFORE_INPUT_PHASE, world, null);
		
		world.runEventIdentifier = runEventIdentifier;
		
		agent.SpawnDebugger(14242, soarJavaDebuggerPath);
		System.out.println(agent.ExecuteCommandLine("source " + agentFolder + agentFileTextField.getText() + ".soar"));
		System.out.println(agent.ExecuteCommandLine("e"));
	}
	
	@Override
	public void runEventHandler(int arg0, Object na, Agent agent, int arg3)
	{		
		soarRunning = true;
		
		if (stopSoar)
		{
			agent.StopSelf();
			soarRunning = false;
			stopSoar = false;
		}
		
		if (resetSoar)
		{
			agent.StopSelf();
			resetSoar = false;
			soarRunning = false;
			
			new Thread(resetSoarRunnable).start();
		}
	}
	
	@Override
	public void actionPerformed(ActionEvent e)
	{
		if (e.getSource() == resetButton)
		{
			if (soarRunning)
				resetSoar = true;
			else
				new Thread(resetSoarRunnable).start();
		}
		else if (e.getSource() == runSoarButton)
		{
			world.sleepMsecs = new Integer(timeDelayTextField.getText());
			stopSoar = false;
			soarRunning = true;
			
			Runnable r = new Runnable()
			{
				@Override
				public void run()
				{
					agent.RunSelfForever();
				}
			};
			
			new Thread(r).start();
		}
		else if (e.getSource() == stopSoarButton)
		{
			stopSoar = true;
		}
	}
}

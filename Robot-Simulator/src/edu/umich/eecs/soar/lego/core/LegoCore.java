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
	
	JLabel		discreteLegoWorldPathLabel		= new JLabel("Discrete Lego World Path: ");
	JLabel		soarDebuggerJarPathLabel		= new JLabel("Soar Debugger Jar Path: ");
	JLabel		agentsFolderLabel				= new JLabel("Agents Folder: ");
	JLabel		agentFileLabel					= new JLabel("Agent File: ");
	JLabel		mapNameLabel					= new JLabel("Map Name: ");
	JLabel		seedLabel						= new JLabel("Seed: ");
	JLabel		timeDelayLabel					= new JLabel("Time Delay (ms): ");
	
	JTextField	discreteLegoWorldPathTextField	= new JTextField("", 30);
	JTextField	soarDebuggerJarPathTextField	= new JTextField("", 30);
	JTextField	agentsFolderTextField			= new JTextField("", 30);
	JTextField	agentFileTextField				= new JTextField("", 15);
	JTextField	mapNameTextField				= new JTextField("", 15);
	JTextField	seedTextField					= new JTextField("", 10);
	JTextField	timeDelayTextField				= new JTextField("", 10);
	JButton		resetButton						= new JButton("Reset");
	JButton		runSoarButton					= new JButton("Run Soar");
	JButton		stopSoarButton					= new JButton("Stop Soar");
	
	boolean		stopSoar						= false;
	boolean		resetSoar						= false;
	boolean		soarRunning						= false;
	
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
		
		resetWindow.getContentPane().add(discreteLegoWorldPathLabel);
		resetWindow.getContentPane().add(soarDebuggerJarPathLabel);
		resetWindow.getContentPane().add(agentsFolderLabel);
		resetWindow.getContentPane().add(agentFileLabel);
		resetWindow.getContentPane().add(mapNameLabel);
		resetWindow.getContentPane().add(seedLabel);
		resetWindow.getContentPane().add(timeDelayLabel);
		
		resetWindow.getContentPane().add(discreteLegoWorldPathTextField);
		resetWindow.getContentPane().add(soarDebuggerJarPathTextField);
		resetWindow.getContentPane().add(agentsFolderTextField);
		resetWindow.getContentPane().add(agentFileTextField);
		resetWindow.getContentPane().add(mapNameTextField);
		resetWindow.getContentPane().add(seedTextField);
		resetWindow.getContentPane().add(timeDelayTextField);
		resetWindow.getContentPane().add(resetButton);
		resetWindow.getContentPane().add(runSoarButton);
		resetWindow.getContentPane().add(stopSoarButton);
		
		layout.putConstraint(SpringLayout.WEST, discreteLegoWorldPathLabel, 5, SpringLayout.WEST, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.WEST, soarDebuggerJarPathLabel, 20, SpringLayout.WEST, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.WEST, agentsFolderLabel, 76, SpringLayout.WEST, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.WEST, agentFileLabel, 100, SpringLayout.WEST, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.WEST, mapNameLabel, 97, SpringLayout.WEST, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.WEST, seedLabel, 134, SpringLayout.WEST, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.WEST, timeDelayLabel, 62, SpringLayout.WEST, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.WEST, runSoarButton, 180, SpringLayout.WEST, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.WEST, stopSoarButton, 270, SpringLayout.WEST, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.WEST, resetButton, 230, SpringLayout.WEST, resetWindow.getContentPane());
		
		layout.putConstraint(SpringLayout.NORTH, discreteLegoWorldPathLabel, 10, SpringLayout.NORTH, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.NORTH, soarDebuggerJarPathLabel, 40, SpringLayout.NORTH, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.NORTH, agentsFolderLabel, 70, SpringLayout.NORTH, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.NORTH, agentFileLabel, 100, SpringLayout.NORTH, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.NORTH, mapNameLabel, 130, SpringLayout.NORTH, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.NORTH, seedLabel, 160, SpringLayout.NORTH, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.NORTH, timeDelayLabel, 190, SpringLayout.NORTH, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.NORTH, runSoarButton, 220, SpringLayout.NORTH, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.NORTH, stopSoarButton, 220, SpringLayout.NORTH, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.NORTH, resetButton, 250, SpringLayout.NORTH, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.SOUTH, resetWindow.getContentPane(), 5, SpringLayout.SOUTH, resetButton);
		
		layout.putConstraint(SpringLayout.WEST, discreteLegoWorldPathTextField, 5, SpringLayout.EAST, discreteLegoWorldPathLabel);
		layout.putConstraint(SpringLayout.WEST, soarDebuggerJarPathTextField, 5, SpringLayout.EAST, soarDebuggerJarPathLabel);
		layout.putConstraint(SpringLayout.WEST, agentsFolderTextField, 5, SpringLayout.EAST, agentsFolderLabel);
		layout.putConstraint(SpringLayout.WEST, agentFileTextField, 5, SpringLayout.EAST, agentFileLabel);
		layout.putConstraint(SpringLayout.WEST, mapNameTextField, 5, SpringLayout.EAST, mapNameLabel);
		layout.putConstraint(SpringLayout.WEST, seedTextField, 5, SpringLayout.EAST, seedLabel);
		layout.putConstraint(SpringLayout.WEST, timeDelayTextField, 5, SpringLayout.EAST, timeDelayLabel);
		
		layout.putConstraint(SpringLayout.NORTH, discreteLegoWorldPathTextField, 5, SpringLayout.NORTH, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.NORTH, soarDebuggerJarPathTextField, 35, SpringLayout.NORTH, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.NORTH, agentsFolderTextField, 65, SpringLayout.NORTH, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.NORTH, agentFileTextField, 95, SpringLayout.NORTH, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.NORTH, mapNameTextField, 125, SpringLayout.NORTH, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.NORTH, seedTextField, 155, SpringLayout.NORTH, resetWindow.getContentPane());
		layout.putConstraint(SpringLayout.NORTH, timeDelayTextField, 185, SpringLayout.NORTH, resetWindow.getContentPane());
		
		layout.putConstraint(SpringLayout.EAST, resetWindow.getContentPane(), 5, SpringLayout.EAST, discreteLegoWorldPathTextField);
		
		resetButton.addActionListener(this);
		runSoarButton.addActionListener(this);
		stopSoarButton.addActionListener(this);
		
		resetWindow.pack();
		resetWindow.setVisible(true);
	}
	
	public void run(String DiscreteLegoWorldPath, Integer TimeDelayMiliseconds, String SoarDebuggerJarPath, String AgentFile, String Map, Integer seed, String agents)
	{
		discreteLegoWorldPathTextField.setText(DiscreteLegoWorldPath);
		timeDelayTextField.setText(TimeDelayMiliseconds.toString());
		soarDebuggerJarPathTextField.setText(SoarDebuggerJarPath);
		agentFileTextField.setText(AgentFile);
		mapNameTextField.setText(Map);
		seedTextField.setText(seed.toString());
		agentsFolderTextField.setText(agents);
		
		startWorld();
	}
	
	private void startWorld()
	{
		map = new LegoMap(discreteLegoWorldPathTextField.getText() + "maps/" + mapNameTextField.getText());
		
		world = new World(map, new Integer(timeDelayTextField.getText()));
		
		kernel = Kernel.CreateKernelInNewThread(14242);
		agent = kernel.CreateAgent("Lego Soar");
		
		System.out.println("Using Seed: " + seedTextField.getText());
		agent.ExecuteCommandLine("srand " + seedTextField.getText());
		agent.RegisterForRunEvent(smlRunEventId.smlEVENT_AFTER_OUTPUT_PHASE, this, null);
		
		long runEventIdentifier = agent.RegisterForRunEvent(smlRunEventId.smlEVENT_BEFORE_INPUT_PHASE, world, null);
		
		long outHandlerIdentifier = agent.AddOutputHandler("motor", world, null);
		
		world.runEventIdentifier = runEventIdentifier;
		world.outputEventIdentifier = outHandlerIdentifier;
		
		agent.SpawnDebugger(14242, soarDebuggerJarPathTextField.getText());
		System.out.println(agent.ExecuteCommandLine("source " + agentsFolderTextField.getText() + "/" + agentFileTextField.getText() + ".soar"));
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

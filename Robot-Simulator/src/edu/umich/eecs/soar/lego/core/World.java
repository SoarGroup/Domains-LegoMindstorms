package edu.umich.eecs.soar.lego.core;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Map.Entry;

import javax.swing.JOptionPane;

import sml.Agent;
import sml.Agent.OutputEventInterface;
import sml.Agent.RunEventInterface;
import sml.Identifier;
import sml.IntElement;
import sml.StringElement;
import sml.WMElement;
import edu.princeton.cs.introcs.Draw;

public class World implements RunEventInterface, OutputEventInterface
{
	// Soar Variables
	private Map<Sensor, Identifier>				robotSensorToSoarIdentifier	= new HashMap<Sensor, Identifier>();
	private Map<Sensor, Map<String, WMElement>>	sensorOutputs				= new HashMap<Sensor, Map<String, WMElement>>();
	
	public long									runEventIdentifier;
	public long									outputEventIdentifier;
	
	// World Variables
	public Robot								robot						= new Robot(new Coordinate(0, 0));
	private LegoMap								map;
	public int									sleepMsecs					= 0;
	private int									timeStep					= 0;
	
	// Drawing Variables
	public Draw									d							= new Draw("Lego World");
	
	public World(LegoMap map, int sleepMsecs)
	{
		robot.setLocation(map.robotStartLocation);
		this.map = map;
		
		d.setCanvasSize(map.getXSize() * 10, map.getYSize() * 10);
		d.setXscale(-1, map.getXSize() + 1);
		d.setYscale(-1, map.getYSize() + 1);
		
		this.sleepMsecs = sleepMsecs;
	}
	
	private void _initSensors(Identifier inputLink)
	{
		for (Sensor sensor : robot.sensors)
		{
			if (sensor != null)
			{
				final Identifier id = robotSensorToSoarIdentifier.get(sensor);
				
				if (id == null)
				{
					robotSensorToSoarIdentifier.put(sensor, inputLink.CreateIdWME("sensor"));
					sensorOutputs.put(sensor, new HashMap<String, WMElement>());
				}
			}
		}
		
		for (Motor motor : robot.motors)
		{
			if (motor != null)
			{
				final Identifier id = robotSensorToSoarIdentifier.get(motor);
				
				if (id == null)
				{
					robotSensorToSoarIdentifier.put(motor, inputLink.CreateIdWME("motor"));
					sensorOutputs.put(motor, new HashMap<String, WMElement>());
				}
			}
		}
	}
	
	private void _updateSensors(Identifier inputLink)
	{
		for (Entry<Sensor, Identifier> eS : robotSensorToSoarIdentifier.entrySet())
		{
			final Sensor sensor = eS.getKey();
			final Identifier sensorIdentifier = eS.getValue();
			
			final Map<String, Object> output = sensor.getOutput(map, robot.getLocation());
			final Map<String, WMElement> wmes = sensorOutputs.get(sensor);
			
			Iterator<Entry<String, WMElement>> it = wmes.entrySet().iterator();
			
			while (it.hasNext())
			{
				Entry<String, WMElement> e = it.next();
				if (!output.containsKey(e.getKey()))
				{
					e.getValue().DestroyWME();
					it.remove();
				}
			}
			
			for (Entry<String, Object> e : output.entrySet())
			{
				String stringValue = "N/A";
				long intValue = Integer.MIN_VALUE;
				double floatValue = Double.MIN_VALUE;
				
				if (wmes.containsKey(e.getKey()))
				{
					WMElement element = wmes.get(e.getKey());
					
					switch (element.GetValueType())
					{
						case "Integer":
							intValue = element.ConvertToIntElement().GetValue();
							break;
						case "String":
							stringValue = element.ConvertToStringElement().GetValue();
							break;
						case "Float":
							floatValue = element.ConvertToFloatElement().GetValue();
							break;
						case "Identifier":
						default:
							break;
					}
					
					wmes.get(e.getKey()).DestroyWME();
				}
				
				if (e.getValue() instanceof String && !e.getValue().equals(stringValue))
					wmes.put(e.getKey(), sensorIdentifier.CreateStringWME(e.getKey(), (String) e.getValue()));
				else if (e.getValue() instanceof Integer && ((Integer) e.getValue()).longValue() != intValue)
					wmes.put(e.getKey(), sensorIdentifier.CreateIntWME(e.getKey(), ((Integer) e.getValue()).longValue()));
				else if (e.getValue() instanceof Double && ((Double) e.getValue()).doubleValue() != floatValue)
					wmes.put(e.getKey(), sensorIdentifier.CreateFloatWME(e.getKey(), ((Double) e.getValue()).doubleValue()));
				else if (e.getValue() instanceof Identifier)
					assert (false);
			}
		}
	}
	
	private void _updateGUI()
	{
		d.clear();
		
		map.draw(d);
		robot.draw(d);
		
		d.show();
		d.show(sleepMsecs);
	}
	
	@Override
	public void runEventHandler(int arg0, Object na, Agent agent, int arg3)
	{
		int t = timeStep++;
		
		robot.update(t);
		
		_initSensors(agent.GetInputLink());
		_updateSensors(agent.GetInputLink());
		_updateGUI();
		
		if (robot.getLocation().equals(map.robotFinishLocation))
		{
			agent.UnregisterForRunEvent(runEventIdentifier);
			agent.UnregisterForOutputNotification(outputEventIdentifier);
			
			JOptionPane.showMessageDialog(d.frame, "Finished!");
		}
	}
	
	@Override
	public void outputEventHandler(Object na, String agentName, String cmd, WMElement wme)
	{
		if (!wme.IsIdentifier())
			return;
		
		final Identifier id = wme.ConvertToIdentifier();
		if (id != null)
		{
			boolean good = false;
			
			String wmeAttribute = wme.GetAttribute();
			
			if (wmeAttribute.equals("motor"))
			{
				Identifier motorId = wme.ConvertToIdentifier();
				if (motorId != null)
				{
					long power = -101;
					int port = -1;
					String direction = "stop";
					
					for (int i = 0; i < motorId.GetNumberChildren(); ++i)
					{
						WMElement child = motorId.GetChild(i);
						
						if (child.GetAttribute().equals("set-power"))
						{
							IntElement e = child.ConvertToIntElement();
							power = e.GetValue();
						}
						else if (child.GetAttribute().equals("port"))
						{
							StringElement es = child.ConvertToStringElement();
							String portString = es.GetValue();
							port = portString.charAt(0) - 'A';
						}
						else if (child.GetAttribute().equals("set-direction"))
						{
							StringElement es = child.ConvertToStringElement();
							direction = es.GetValue();
						}
						else
						{
							motorId.CreateStringWME("error", "Invalid motor command: " + child.GetAttribute());
							motorId.AddStatusError();
						}
					}
					
					if (direction.equals("stop"))
						power = 0;
					else if (direction.equals("backward"))
						power *= 1;
					else if (direction.equals("forward"))
						power *= -1;
					else
					{
						power = -101;
						motorId.CreateStringWME("error", "Invalid motor direction: " + direction);
						motorId.AddStatusError();
					}
					
					if (port >= 0 && power >= -100 && power <= 100 && port < 2)
					{
						good = true;
						
						robot.motors[port].setPower(power);
					}
				}
				else
					id.CreateStringWME("error", "Invalid motor command");
				
				good = true;
			}
			else
				id.CreateStringWME("error", "Unhandled command: " + wmeAttribute);
			
			if (good)
				id.AddStatusComplete();
			else
				id.AddStatusError();
		}
	}
}

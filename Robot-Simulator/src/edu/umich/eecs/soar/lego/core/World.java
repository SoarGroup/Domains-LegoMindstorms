package edu.umich.eecs.soar.lego.core;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Map.Entry;

import javax.swing.JOptionPane;

import com.itextpdf.text.BadElementException;
import com.itextpdf.text.Document;
import com.itextpdf.text.DocumentException;
import com.itextpdf.text.Rectangle;
import com.itextpdf.text.pdf.PdfWriter;

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
	private Identifier simulatorIdentifier = null;
	
	public long									runEventIdentifier;
	public long									outputEventIdentifier;
	
	// World Variables
	public Robot								robot						= new Robot(new Coordinate(0, 0));
	private LegoMap								map;
	public int									sleepMsecs					= 0;
	private int									timeStep					= 0;
	
	// Drawing Variables
	public Draw									d							= new Draw("Lego World");
	
	public double scaleFactor = 1.0;//2.83*1.30;
	public boolean renderPDF = false;
	
	public World(LegoMap map, int sleepMsecs)
	{
		robot.setLocation(map.robotStartLocation);
		this.map = map;
		
		d.setCanvasSize((int)(map.getXSize() * 10 * scaleFactor), (int)(map.getYSize() * 10 * scaleFactor));
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
		if (simulatorIdentifier == null)
			simulatorIdentifier = inputLink.CreateIdWME("simulator");
		
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
		
		if (renderPDF)
		{
			int width = d.onscreenImage.getWidth();
			int height = d.onscreenImage.getHeight();
			
			Document doc = new Document(new Rectangle((int)(d.onscreenImage.getWidth()), (int)(d.onscreenImage.getHeight())));
			
			try
			{
				PdfWriter.getInstance(doc, new FileOutputStream(map.mapFile + ".pdf"));
			}
			catch (FileNotFoundException e1)
			{
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
			catch (DocumentException e1)
			{
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
			
			doc.open();
			
			try
			{
				com.itextpdf.text.Image image = com.itextpdf.text.Image.getInstance(d.onscreenImage.getScaledInstance(width, height, 0), null);
				image.setAbsolutePosition(0, 0);
				doc.add(image);
			}
			catch (BadElementException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			catch (IOException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			catch (DocumentException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
			doc.close();
		}
		
		robot.draw(d);
		
		d.show();
		d.show(sleepMsecs);
	}
	
	@Override
	public void runEventHandler(int arg0, Object na, Agent agent, int arg3)
	{
		System.out.println("Run Event Handler World");
		
		Identifier id = agent.GetOutputLink();
		
		if (id != null)
		{
			for (int i = 0;i < id.GetNumberChildren();++i)
			{
				WMElement wme = id.GetChild(i);
				
				if (wme.IsJustAdded())
					outputEventHandler(null, agent.GetAgentName(), wme.GetAttribute().toString(), wme);
			}
		}
		
		int t = timeStep++;
		
		robot.update(t);
		
		_initSensors(agent.GetInputLink());
		_updateSensors(agent.GetInputLink());
		_updateGUI();
		
		if (robot.getLocation().equals(map.robotFinishLocation))
		{			
			agent.UnregisterForRunEvent(runEventIdentifier);
			//			agent.UnregisterForOutputNotification(outputEventIdentifier);
			
			JOptionPane.showMessageDialog(d.frame, "Finished!");
		}
	}
	
	@Override
	public void outputEventHandler(Object na, String agentName, String cmd, WMElement wme)
	{
		System.out.println("Output handler called");
		
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
						power *= -1;
					else if (direction.equals("forward"))
						power *= 1;
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

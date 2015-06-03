package edu.umich.eecs.soar.lego.core;

import java.awt.Color;

import edu.princeton.cs.introcs.Draw;
import edu.umich.eecs.soar.lego.sensors.ColorSensor;
import edu.umich.eecs.soar.lego.sensors.TouchSensor;

public class Robot
{
	private Coordinate			currentCoord;
	private int					rotation;
	
	public Sensor				sensors[];
	public Motor				motors[];
		
	public Robot(Coordinate startCoord)
	{
		this.currentCoord = startCoord;
		this.rotation = 0;
		
		motors = new Motor[] { new Motor("A", 0), new Motor("B", 0), null, null };
		sensors = new Sensor[] { new ColorSensor(1, -2, 0), new ColorSensor(2, 2, 0), new ColorSensor(3, 0, 0), new TouchSensor(4, 0, 3.5) };
	}
	
	public void setLocation(Coordinate location)
	{
		currentCoord = location;
	}
	
	public Coordinate getLocation()
	{
		return currentCoord;
	}
	
	public void update(double time)
	{
		// Negative = rotation to the left
		if (motors[0].getPower() < motors[1].getPower())
		{
			rotation += 45;
			
			if (rotation >= 360)
				rotation -= 360;
		}
		else if (motors[0].getPower() > motors[1].getPower())
		{
			rotation -= 45;
			
			if (rotation < 0)
				rotation += 360;
		}
		else if (motors[0].getPower() != 0 && motors[1].getPower() != 0)
		{			
			double xMovement = 2;
			double yMovement = 2;
			
			TouchSensor touch = ((TouchSensor)sensors[3]);
			
			switch (rotation)
			{
				case 0:
					xMovement = 0;
					yMovement *= (touch.currentState ? 0 : 1);
					break;
				case 90:
					yMovement = 0;
					xMovement *= (touch.currentState ? 0 : 1);
					break;
				case 180:
					xMovement = 0;
					yMovement *= -1 * (touch.currentState ? 0 : 1);
					break;
				case 270:
					yMovement = 0;
					xMovement *= -1 * (touch.currentState ? 0 : 1);
					break;
				default:
					assert (false);
			}
			
			currentCoord.set(currentCoord.getX() + xMovement, currentCoord.getY() + yMovement);
		}
		
		double offsetX = 2, offsetY = 0;
		
		switch (rotation)
		{
			case 0:
				break;
			case 45:
				offsetY = -2;
				break;
			case 90:
				offsetX = 0;
				offsetY = -2;
				break;
			case 135:
				offsetX = -2;
				offsetY = -2;
				break;
			case 180:
				offsetX = -2;
				break;
			case 225:
				offsetX = -2;
				offsetY = 2;
				break;
			case 270:
				offsetX = 0;
				offsetY = 2;
				break;
			case 315:
				offsetX = 2;
				offsetY = 2;
				break;
			default:
				assert (false);
		}
		
		((ColorSensor)this.sensors[0]).centerOffsetX = offsetX;
		((ColorSensor)this.sensors[0]).centerOffsetY = offsetY;
		
		((ColorSensor)this.sensors[1]).centerOffsetX = offsetX;
		((ColorSensor)this.sensors[1]).centerOffsetY = offsetY;
		
		((TouchSensor)this.sensors[3]).centerOffsetX = offsetY * -4/2.0;
		((TouchSensor)this.sensors[3]).centerOffsetY = offsetX * 4/2.0;
	}
	
	public void draw(Draw d)
	{
		double x = currentCoord.getX() - 0.05, y = currentCoord.getY();
		
		// Main Body
		d.setPenColor(Color.CYAN);
		d.rectangle(x, y, 3.5, 3.5);
				
		switch (rotation)
		{
			case 0:
				d.line(x, y, x, y + 3.5);
				break;
			case 45:
				d.line(x, y, x + 3.5, y + 3.5);
				break;
			case 90:
				d.line(x, y, x + 3.5, y);
				break;
			case 135:
				d.line(x, y, x + 3.5, y - 3.5);
				break;
			case 180:
				d.line(x, y, x, y - 3.5);
				break;
			case 225:
				d.line(x, y, x - 3.5, y - 3.5);
				break;
			case 270:
				d.line(x, y, x - 3.5, y);
				break;
			case 315:
				d.line(x, y, x - 3.5, y + 3.5);
				break;
			default:
				assert (false);
		}
		
		d.setPenColor(Color.RED);
		d.filledRectangle(x, y, 0.5, 0.5);
		
		d.setPenColor(Color.CYAN);
		d.filledRectangle(x - ((ColorSensor)sensors[0]).centerOffsetX, y - ((ColorSensor)sensors[0]).centerOffsetY, 0.75, 0.75);
		d.filledRectangle(x + ((ColorSensor)sensors[1]).centerOffsetX, y + ((ColorSensor)sensors[1]).centerOffsetY, 0.75, 0.75);
		d.filledRectangle(x, y, 0.75, 0.75);

		d.setPenColor(((ColorSensor)sensors[0]).currentColor.color);
		d.filledRectangle(x - ((ColorSensor)sensors[0]).centerOffsetX, y - ((ColorSensor)sensors[0]).centerOffsetY, 0.5, 0.5);
			
		d.setPenColor(((ColorSensor)sensors[1]).currentColor.color);
		d.filledRectangle(x + ((ColorSensor)sensors[1]).centerOffsetX, y + ((ColorSensor)sensors[1]).centerOffsetY, 0.5, 0.5);
	
		d.setPenColor(((ColorSensor)sensors[2]).currentColor.color);
		d.filledRectangle(x, y, 0.5, 0.5);
		
		TouchSensor touch = ((TouchSensor)sensors[3]);
		d.setPenColor(touch.currentState ? Color.RED : Color.GREEN);
		d.filledRectangle(x + touch.centerOffsetX, y + touch.centerOffsetY, 0.5, 0.5);
	}
}

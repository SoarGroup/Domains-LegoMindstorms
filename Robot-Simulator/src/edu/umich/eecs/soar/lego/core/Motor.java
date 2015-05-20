package edu.umich.eecs.soar.lego.core;

import java.util.Map;
import java.util.TreeMap;

public class Motor implements Sensor
{
	private static final double			rotationsPerPowerTime	= 1.0;
	private final Map<String, Object>	output					= new TreeMap<String, Object>();
	
	private long						power;
	private double						rotations;
	private int						speed;
	
	public Motor(String port, int power)
	{
		this.power = power;
		rotations = 0.0;
		speed = 0;
		
		output.put("port", new String(port));
	}
	
	public void setPower(long power2)
	{
		if (power2 >= 100)
			this.power = 100;
		else if (power2 <= -100)
			this.power = -100;
		else
			this.power = power2;
	}
	
	public long getPower()
	{
		return power;
	}
	
	public double getRotations()
	{
		return rotations;
	}
	
	public void update(double time)
	{
		rotations += rotationsPerPowerTime * power * time;
		
		if (power > 0)
			speed = 1;
		else if (power < 0)
			speed = -1;
		else
			speed = 0;
	}
	
	public Map<String, Object> getOutput(LegoMap map, Coordinate coord)
	{
		output.put("speed", speed);
		output.put("amount-rotated", new Double(rotations));
		output.put("direction", speed == -1 ? "forward" : speed == 1 ? "backward" : "stopped");
		
		return output;
	}
}

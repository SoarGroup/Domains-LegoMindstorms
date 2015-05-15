package edu.umich.eecs.soar.lego.core;

import java.util.Map;

public interface Sensor
{
	public Map<String, Object> getOutput(LegoMap map, Coordinate coord);
}

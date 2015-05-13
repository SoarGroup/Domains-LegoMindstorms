package edu.umich.eecs.soar.lego.sensors;

import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import edu.umich.eecs.soar.lego.core.Coordinate;
import edu.umich.eecs.soar.lego.core.LegoMap;
import edu.umich.eecs.soar.lego.core.Sensor;
import edu.umich.eecs.soar.lego.core.Tile;
import edu.umich.eecs.soar.lego.core.Tile.TileBlocked;

public class TouchSensor implements Sensor
{
	private final Map<String, Object>	output			= new TreeMap<String, Object>();
	public boolean						currentState	= false;
	public boolean						previousState	= false;
	
	public double centerOffsetX = 0.0;
	public double centerOffsetY = 0.0;
	
	public TouchSensor(int port, double x, double y)
	{
		output.put("port", port);
		output.put("type", "touch");
		output.put("previous-state", "released");
		
		centerOffsetX = x;
		centerOffsetY = y;
	}
	
	private boolean isPressed(LegoMap map, Coordinate coord)
	{
		Coordinate testCoord = new Coordinate(coord.getX() + centerOffsetX, coord.getY() + centerOffsetY);
		List<Tile> tiles = map.getTilesAt(testCoord);
		
		if (!tiles.isEmpty() && tiles.get(0).blocked == TileBlocked.blocked)
			return true;
		else
			return false;
	}
	
	@Override
	public Map<String, Object> getOutput(LegoMap map, Coordinate coord)
	{
		previousState = currentState;
		
		currentState = isPressed(map, coord);
		
		output.put("current-state", currentState ? "pressed" : "released");
		output.put("previous-state", previousState ? "pressed" : "released");
		
		return output;
	}
}

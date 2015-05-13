package edu.umich.eecs.soar.lego.sensors;

import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import edu.umich.eecs.soar.lego.core.Coordinate;
import edu.umich.eecs.soar.lego.core.LegoMap;
import edu.umich.eecs.soar.lego.core.Sensor;
import edu.umich.eecs.soar.lego.core.Tile;
import edu.umich.eecs.soar.lego.core.Tile.TileColor;

public class ColorSensor implements Sensor
{	
	private final Map<String, Object>					output	= new TreeMap<String, Object>();
	public double centerOffsetX, centerOffsetY;
	public TileColor currentColor;
	
	public ColorSensor(int port, double centerOffsetX, double centerOffsetY)
	{
		output.put("port", port);
		output.put("type", "color");
		output.put("mode", "color");
		
		this.centerOffsetX = centerOffsetX;
		this.centerOffsetY = centerOffsetY;
	}
	
	private TileColor tileColorForCell(LegoMap map, Coordinate coord)
	{
		Coordinate testCoord = new Coordinate(coord.getX() + centerOffsetX, coord.getY() + centerOffsetY);
		List<Tile> tiles = map.getTilesAt(testCoord);
		
		if (!tiles.isEmpty())
			currentColor = tiles.get(0).colorAt(testCoord);
		else
			currentColor = TileColor.white;
		
		return currentColor;
	}
	
	private String colorForCell(LegoMap map, Coordinate coord)
	{
		return tileColorForCell(map, coord).name();
	}
		
	@Override
	public Map<String, Object> getOutput(LegoMap map, Coordinate coord)
	{
		output.put("value", colorForCell(map, coord));
		
		return output;
		
	}
}

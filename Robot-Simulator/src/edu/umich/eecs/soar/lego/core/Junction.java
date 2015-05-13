package edu.umich.eecs.soar.lego.core;

import java.util.ArrayList;

import edu.princeton.cs.introcs.Draw;

public class Junction extends Tile
{
	private ArrayList<Tile> tiles;
	
	public Junction(Coordinate coord, TileColor color)
	{
		super(coord, color, TileBlocked.unblocked);
		
		this.size = 8;
		
		double x = coord.getX(), y = coord.getY();
		
		tiles = new ArrayList<Tile>();
		
		tiles.add(new Tile(new Coordinate(x - 3, y), TileColor.black, TileBlocked.unblocked));
		tiles.add(new Tile(new Coordinate(x + 3, y), TileColor.black, TileBlocked.unblocked));
		tiles.add(new Tile(new Coordinate(x, y - 3), TileColor.black, TileBlocked.unblocked));
		tiles.add(new Tile(new Coordinate(x, y + 3), TileColor.black, TileBlocked.unblocked));
		
		tiles.add(new Tile(new Coordinate(x - 1, y), TileColor.black, TileBlocked.unblocked));
		tiles.add(new Tile(new Coordinate(x + 1, y), TileColor.black, TileBlocked.unblocked));
		tiles.add(new Tile(new Coordinate(x, y - 1), TileColor.black, TileBlocked.unblocked));
		tiles.add(new Tile(new Coordinate(x, y + 1), TileColor.black, TileBlocked.unblocked));
	}
	
	@Override
	public void draw(Draw d)
	{
		super.draw(d);
		
		for (Tile t : tiles)
			t.draw(d);
	}
	
	@Override
	public TileColor colorAt(Coordinate testCoord)
	{
		for (Tile t : tiles)
			if (t.contains(testCoord))
				return t.colorAt(testCoord);
		
		return colorAt(testCoord);
	}
}

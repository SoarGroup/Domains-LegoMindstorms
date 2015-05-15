package edu.umich.eecs.soar.lego.core;

import java.awt.Color;

import edu.princeton.cs.introcs.Draw;

public class Tile
{
	public static enum TileColor
	{
		none(Color.WHITE),
		black(Color.BLACK),
		blue(Color.BLUE),
		green(Color.GREEN),
		yellow(Color.YELLOW),
		red(Color.RED),
		white(Color.WHITE),
		brown(new Color(150, 75, 0)),
		start(Color.LIGHT_GRAY),
		finish(Color.LIGHT_GRAY),
		;
		
		private TileColor(Color color)
		{
			this.color = color;
		}
		
		public final Color	color;
	}
	
	public static enum TileBlocked
	{
		unblocked, blocked, ;
	}
	
	public Tile(Coordinate coord, TileColor color, TileBlocked blocked)
	{
		this.coord = coord;
		this.tileColor = color;
		this.blocked = blocked;
		
		this.size = 2;
	}
	
	public final Coordinate		coord;
	protected int				size;
	
	public TileColor		tileColor;
	public TileBlocked	blocked;
	
	public void draw(Draw d)
	{
		d.setPenColor(tileColor.color);
		d.filledSquare(coord.getX(), coord.getY(), size / 2.0);
		
		if (blocked == TileBlocked.blocked)
		{
			d.setPenColor(Color.BLACK);
			d.line(	coord.getX() - size / 2.0 + 0.2,
					coord.getY() - size / 2.0 + 0.2,
					coord.getX() + size / 2.0 - 0.2,
					coord.getY() + size / 2.0 - 0.2);
			d.line(	coord.getX() - size / 2.0 + 0.2,
					coord.getY() + size / 2.0 - 0.2,
					coord.getX() + size / 2.0 - 0.2,
					coord.getY() - size / 2.0 + 0.2);
			d.rectangle(coord.getX(), coord.getY(), size / 2.0 - 0.10, size / 2.0 - 0.10);
		}
	}
	
	public boolean contains(Coordinate testCoord)
	{
		Coordinate modifiable = new Coordinate(	testCoord.getX(),
												testCoord.getY());
		
		modifiable.setX(modifiable.getX() - coord.getX());
		modifiable.setY(modifiable.getY() - coord.getY());
		
		return (modifiable.getX() >= 0 && modifiable.getX() <= size) && (modifiable.getY() >= 0 && modifiable.getY() <= size);
	}
	
	public TileColor colorAt(Coordinate testCoord)
	{
		if (contains(testCoord))
			return tileColor;
		else
			return TileColor.none;
	}
}

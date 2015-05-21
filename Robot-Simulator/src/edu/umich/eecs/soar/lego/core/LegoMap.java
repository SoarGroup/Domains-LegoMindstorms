package edu.umich.eecs.soar.lego.core;

import java.awt.Color;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import com.itextpdf.text.Document;
import com.itextpdf.text.Rectangle;

import edu.princeton.cs.introcs.Draw;
import edu.umich.eecs.soar.lego.core.Tile.TileBlocked;
import edu.umich.eecs.soar.lego.core.Tile.TileColor;

public class LegoMap
{
	private int							xSize;
	private int							ySize;
	private ArrayList<ArrayList<ArrayList<Tile>>>	tiles;
	private ArrayList<Tile>				allTiles;
	
	public Coordinate robotStartLocation = new Coordinate(1,1);
	public Coordinate robotFinishLocation = new Coordinate(9,9);
	
	// 1 unit = 1 cm
	public LegoMap(int xSize, int ySize)
	{
		this.xSize = xSize;
		this.ySize = ySize;
		
		_init();
	}
	
	private char[][] create2DCharMatrixFromFile(String filename)
	{
		try
		{
			char[][] matrix = null;
			
			BufferedReader buffer = new BufferedReader(new FileReader(filename));
			
			String line;
			int row = 0;
			
			line = buffer.readLine();
			
			xSize = Integer.parseInt(line);
			line = buffer.readLine();
			ySize = Integer.parseInt(line);
			
			matrix = new char[ySize][xSize];
			
			while ((line = buffer.readLine()) != null && row < ySize)
			{
				String map = line.trim();
				
				for (int col = 0; col < xSize; col++)
					matrix[row][col] = map.charAt(col);
				
				++row;
			}
			
			buffer.close();
			
			xSize *= 2;
			ySize *= 2;
			
			return matrix;
		}
		catch (IOException e)
		{
			e.printStackTrace();
			assert (false);
		}
		return null;
	}
	
	private ArrayList<Tile> parseCharToMap(char map[][])
	{
		ArrayList<Tile> tiles = new ArrayList<Tile>();
		
		for (int row = 0; row < ySize / 2; ++row)
		{
			for (int col = 0; col < xSize / 2; ++col)
			{
				Tile t = new Tile(new Coordinate(col * 2 + 1, (ySize / 2 - row) * 2 - 1), TileColor.white, TileBlocked.unblocked);
				
				switch (map[row][col] - '0')
				{
					case 1: // Black
						t.tileColor = TileColor.black;
						break;
					case 2: // Blue
						t.tileColor = TileColor.blue;
						break;
					case 3: // Green
						t.tileColor = TileColor.green;
						break;
					case 4: // Yellow
						t.tileColor = TileColor.yellow;
						break;
					case 5: // Red
						t.tileColor = TileColor.red;
						break;
					case 7: // Brown
						t.tileColor = TileColor.brown;
						break;
					case 9: // Blocked
						t.tileColor = TileColor.white;
						t.blocked = TileBlocked.blocked;
						break;
					case 6: // Start
						t.tileColor = TileColor.black;
						robotStartLocation = new Coordinate (t.coord.getX(), t.coord.getY());
						break;
					case 8: // Finish
						t.tileColor = TileColor.finish;
						robotFinishLocation = new Coordinate (t.coord.getX(), t.coord.getY());
						break;
					default:
						continue;
				}
				
				tiles.add(t);
			}
		}
		
		return tiles;
	}
	
	public LegoMap(String filename)
	{
		// 1 char = 2cm
		char map[][] = create2DCharMatrixFromFile(filename);
		ArrayList<Tile> tiles = parseCharToMap(map);
		
		_init();
		
		for (Tile t : tiles)
			this.addTile(t);
	}
	
	private void _init()
	{
		this.allTiles = new ArrayList<Tile>();
		
		tiles = new ArrayList<ArrayList<ArrayList<Tile>>>();
		
		for (int row = 0; row <= ySize; ++row)
		{
			ArrayList<ArrayList<Tile>> rowTiles = new ArrayList<ArrayList<Tile>>();
			
			for (int col = 0; col <= xSize; ++col)
				rowTiles.add(new ArrayList<Tile>());
			
			tiles.add(rowTiles);
		}
	}
	
	public int getXSize()
	{
		return xSize;
	}
	
	public int getYSize()
	{
		return ySize;
	}
	
	public void addTile(Tile t)
	{
		assert (t.coord.getX() >= 0 && t.coord.getX() < xSize && t.coord.getY() >= 0 && t.coord.getY() < ySize);
		
		tiles.get((int)t.coord.getY()).get((int)t.coord.getX()).add(0, t);		
		allTiles.add(t);
	}
	
	public List<Tile> getTilesAt(Coordinate coord)
	{
		assert (coord.getX() >= 0 && coord.getX() < xSize && coord.getY() >= 0 && coord.getY() < ySize);
		
		return tiles.get((int)coord.getY()).get((int)coord.getX());
	}
	
	public void draw(Draw d)
	{
		d.setPenRadius(0.006);
		
		for (Tile t : allTiles)
			t.draw(d);
		
		d.setPenColor(Color.GRAY);
		
		// Draw the border
		d.rectangle(xSize / 2.0, ySize / 2.0, xSize / 2.0, ySize / 2.0);
		
		int width = d.onscreenImage.getWidth();
		int height = d.onscreenImage.getHeight();
		
		Document doc = new Document(new Rectangle(d.onscreenImage.getWidth(), d.onscreenImage.getHeight()));
		
        com.itextpdf.text.Image image = com.itextpdf.text.Image.getInstance(d.onscreenImage.getScaledInstance(width, height, 0));
	}
}

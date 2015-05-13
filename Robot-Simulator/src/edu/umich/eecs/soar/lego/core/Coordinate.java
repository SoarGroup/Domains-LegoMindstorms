package edu.umich.eecs.soar.lego.core;

public class Coordinate
{
	public Coordinate(double x, double y)
	{
		this.x = x;
		this.y = y;
	}
	
	public double getX() { return x; }
	public double getY() { return y; }
	
	public void setX(double x) { this.x = x; }
	public void setY(double y) { this.y = y; }
	
	public void set(double x, double y) { this.x = x; this.y = y; }
	
	public boolean equals(Coordinate other) { return other.x == this.x && other.y == this.y; }
	
	private double x, y;
}

/*
 * WMUtil.h
 *
 *  Created on: Oct 10, 2013
 *      Author: aaron
 *  Helper functions for accessing and modifying working memory
 */

#ifndef WMUTIL_H_
#define WMUTIL_H_


#include <string>
#include <vector>

#include "sml_Client.h"

using namespace std;
using namespace sml;

// These are the three types that a WME can be, conform to Soar
// (Match the strings returned by WMElement::GetValueType())
extern const string INTEGER_VAL; // Can be of long precision
extern const string FLOAT_VAL;   // Can be of double precision
extern const string STRING_VAL;

class WMUtil{
public:
	/*****
	 * Returns true if the given string is an integer
	 */
	static bool isint(const string& s);

	/*****
	 * Returns true if the given string is a float
	 */
	static bool isfloat(const string& s);

	/**
	 * Returns the type of the given string, one of the following:
   * INTEGER_VAL, FLOAT_VAL, or STRING_VAL
	 */
	static  string getValueTypeOfString(const string& s);

	/**
   * Creates the following FloatWME connected to the given identifier:
	 *   (id ^att val)
   * If there is an existing WME with the same id and attribute, it is removed
   *   (Not recommended for multi-valued attributes)
	 */
	static void updateFloatWME(Identifier* id, const string& att, double val);

	/**
   * Creates the following IntWME connected to the given identifier:
	 *   (id ^att val)
   * If there is an existing WME with the same id and attribute, it is removed
   *   (Not recommended for multi-valued attributes)
	 */
	static void updateIntWME(Identifier* id, const string& att, long val);

	/**
   * Creates the following StringWME connected to the given identifier:
	 *   (id ^att val)
   * If there is an existing WME with the same id and attribute, it is removed
   *   (Not recommended for multi-valued attributes)
	 */
	static void updateStringWME(Identifier* id, const string& att, const string& val);

	/**
   * Creates the following wme connected to the given identifier:
	 *   (id ^att val)
   * The type of the WME will be dependent on the given val
   *   (i.e. it checks the type of the string and creates the appropriate wme, 
   *         so it will create an IntWME if the val string is a valid integer)
   * If there is an existing WME with the same id and attribute, it is removed
   *   (Not recommended for multi-valued attributes)
	 */
	static void updateWME(Identifier* id, const string& att, const string& val);

	/**
	 * If the given identifier has the given attribute, the associated WME will
	 * be destroyed (only the first)
	 */
	static void removeWME(Identifier* id, const string& att);

	/**
	 * Given id and attribute, checks for a StringWME of (id ^attribute value)
   *   and assigns the result to value
	 * Returns true if successful (such a wme is found)
	 */
	static bool getValue(Identifier* id, const string& att, string& value);

	/**
	 * Given id and attribute, checks for an IntWME of (id ^attribute value)
   *   and assigns the result to value
	 * Returns true if successful (such a wme is found)
	 */
	static bool getValue(Identifier* id, const string& att, int& value);

	/**
	 * Given id and attribute, checks for an intWME of (id ^attribute value)
   *   and assigns the result to value
	 * Returns true if successful (such a wme is found)
	 */
	static bool getValue(Identifier* id, const string& att, long& value);

	/**
	 * Given id and attribute, checks for a FloatWME of (id ^attribute value)
   *   and assigns the result to value
	 * Returns true if successful (such a wme is found)
	 */
	static bool getValue(Identifier* id, const string& att, double& value);

	/**
	 * Given id and attribute, checks for a WME of (id ^attribute value) with an Identifier value
   *   and assigns the result to value
	 * Returns true if successful (such a wme is found)
	 */
	static bool getValue(Identifier* id, const string& att, Identifier*& value);


	/**
	 * Given id and attribute, returns a set of mutli-valued attributes for each WME matching
	 * (id ^attribute value)
	 */
	static void getAllValuesOfAttribute(Identifier* id, const string& att, vector<string>& values);

	/**
	 * Given id and attribute, returns a set of mutli-valued attributes for each WME matching
	 * (id ^attribute value)
	 */
	static void getAllValuesOfAttribute(Identifier* id, const string& att, vector<double>& values);

	/**
	 * Given id and attribute, returns a set of mutli-valued attributes for each WME matching
	 * (id ^attribute value)
	 */
	static void getAllValuesOfAttribute(Identifier* id, const string& att, vector<int>& values);

};

#endif /* WMUTIL_H_ */

/* 
 * Proview   Open Source Process Control.
 * Copyright (C) 2005-2012 SSAB EMEA AB.
 *
 * This file is part of Proview.
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with Proview. If not, see <http://www.gnu.org/licenses/>
 *
 * Linking Proview statically or dynamically with other modules is
 * making a combined work based on Proview. Thus, the terms and 
 * conditions of the GNU General Public License cover the whole 
 * combination.
 *
 * In addition, as a special exception, the copyright holders of
 * Proview give you permission to, from the build function in the
 * Proview Configurator, combine Proview with modules generated by the
 * Proview PLC Editor to a PLC program, regardless of the license
 * terms of these modules. You may copy and distribute the resulting
 * combined work under the terms of your choice, provided that every 
 * copy of the combined work is accompanied by a complete copy of 
 * the source code of Proview (the version used to produce the 
 * combined work), being distributed under the terms of the GNU 
 * General Public License plus this exception.
 */

package jpwr.jopg;
import jpwr.rt.*;
import java.io.*;
import java.util.*;

public class GrowGroup extends GrowNode {

    public GrowGroup(GrowCmn cmn) {
	super(cmn);
    }

    public int type() {
	return Glow.eObjectType_GrowGroup;
    }

    public void open(BufferedReader reader) {
	String line;
	StringTokenizer token;
	boolean end_found = false;

	try {
	    while( (line = reader.readLine()) != null) {
		token = new StringTokenizer(line);
		int key = new Integer(token.nextToken()).intValue();
		if ( cmn.debug) System.out.println( "GrowGroup : " + line);

		switch ( key) {
		case Glow.eSave_GrowGroup: 
		    break;
		case Glow.eSave_GrowGroup_grownode_part: 
		    super.open(reader); 
		    break;
		case Glow.eSave_GrowGroup_nc: 
		    nc = new GlowNodeGroup(cmn);
		    nc.open(reader);
		    break;
		case Glow.eSave_End:
		    end_found = true;
		    break;
		default:
		    System.out.println( "Syntax error in GrowGroup");
		    break;
		}
		if ( end_found)
		    break;
	    }
		
	} catch ( Exception e) {
	    System.out.println( "IOException GrowGroup");
	}
    }

    public GlowArrayElem get_object_group( GlowArrayElem object) {
	int sts;
	GlowArrayElem group;

	for ( int i = 0; i < nc.a.size(); i++) {
	    if ( nc.a.get(i).type() == Glow.eObjectType_GrowGroup) {
		group = ((GrowGroup)nc.a.get(i)).get_object_group( object);
		if ( group != null)
		    return group;
	    }
	    else if ( nc.a.get(i) == object) {
		return this;
	    }
	}
	return null;
    }

    public Vector<GlowArrayElem> get_object_list() {
	return nc.a;
    }

    public void draw() {
	super.draw();
    }
}
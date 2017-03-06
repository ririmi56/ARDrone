#include "Drone.cpp"
#include "Waypoint.cpp"
#include "Instruction.cpp"
#include "tinystr.h"
#include "tinyxml.h"
#include <iostream>
#include <list>
#include <string>

#define INSTRUCTION_MAX 15
#define WAYPOINT_MAX 10

using namespace std;

TiXmlDocument xmlload(string fileName);
Instruction getInsByID(Instruction tab[], int id);
void sendInst(Instruction inst, Drone *AR, Waypoint WPtab[]);

int main(){
	Drone AR;
	int i=0, id=0;
	Waypoint WPtab[WAYPOINT_MAX];
	Instruction insTab[INSTRUCTION_MAX];
	Instruction currInst;
	//Chargement du xml	
	TiXmlDocument doc = xmlload("Flight_plan.xml");
	
	//Definition du nœud waypoints
	TiXmlHandle docHandle( &doc );
	TiXmlElement* elem = docHandle.FirstChild( "FlightPlan" ).FirstChild( "waypoints" ).FirstChild( "waypoint" ).ToElement();
	if(!elem){
		cerr << "le nœud à atteindre n'existe pas" << endl;
		return 2;
	}
	
	//lectur des waypoints et stockage dans un tableau
	while (elem){
		WPtab[i].nom = elem->Attribute("name");
		elem->QueryIntAttribute("x", &WPtab[i].x);
		elem->QueryIntAttribute("y", &WPtab[i].y);
		elem->QueryIntAttribute("alt", &WPtab[i].alt);
		
		i++;
		elem = elem->NextSiblingElement(); //iteration 
	}
	
	//for(i=0; i<10; i++) cout<<"nom="<<WPtab[i].nom<<"  x="<<WPtab[i].x<<"  y="<<WPtab[i].y<<"  alt="<<WPtab[i].alt<<endl;
	
	elem = docHandle.FirstChild( "FlightPlan" ).FirstChild( "instructions" ).FirstChild( "instruction" ).ToElement();
	if(!elem){
		cerr << "le nœud à atteindre n'existe pas" << endl;
		return 2;
	}
	
	i=0;
	while (elem){
		//cout<<elem->Attribute("type")<<endl;
		insTab[i].type = elem->Attribute("type");
		elem->QueryIntAttribute("id", &insTab[i].id);
		if(elem->Attribute("arg1")!=NULL) insTab[i].arg1 = elem->Attribute("arg1");
		if(elem->Attribute("arg2")!=NULL) insTab[i].arg2 = elem->Attribute("arg2");
		if(elem->Attribute("arg3")!=NULL) insTab[i].arg3 = elem->Attribute("arg3");
		
		i++;
		elem = elem->NextSiblingElement(); //iteration 
	}
	
	//for(i=0; i<INSTRUCTION_MAX; i++) cout<<"type="<<insTab[i].type<<"  id="<<insTab[i].id<<"  arg1="<<insTab[i].arg1<<"  arg2="<<insTab[i].arg2<<endl;
	
	while(getInsByID(insTab, id).id != -1){
		currInst=getInsByID(insTab, id);
		id++;
		cout<<"type="<<currInst.type<<"  id="<<currInst.id<<"  arg1="<<currInst.arg1<<"  arg2="<<currInst.arg2<<endl;
		sendInst(currInst, &AR, WPtab);
	}
	
	
	/*AR.takeOff();
	AR.tempo(5000);
	AR.forward(2300);
	AR.tempo(3000);
	AR.rotRight(1400);
	AR.tempo(3000);
	AR.forward(1000);
	AR.tempo(2000);
	AR.land();*/
}

TiXmlDocument xmlload(string fileName){
	
	TiXmlDocument doc(fileName);
	if(!doc.LoadFile()){
		cerr << "erreur lors du chargement" << endl;
		cerr << "error #" << doc.ErrorId() << " : " << doc.ErrorDesc() << endl;
		exit(1);
	}
	
	return doc;
}

Instruction getInsByID (Instruction tab[], int id){
	
	int i;
	Instruction Null;
	
	for(i=0; i<INSTRUCTION_MAX; i++){
		if(tab[i].id==id){
			return tab[i];
		}
	}
	return Null;
}

Waypoint findWaypoint (Waypoint tab[], string name){
	
	int i;
	Waypoint Null;
	
	for(i=0; i<WAYPOINT_MAX; i++){
		if(tab[i].nom==name){
			return tab[i];
		}
	}
	return Null;
}

void sendInst(Instruction inst, Drone *AR, Waypoint WPtab[]){
	
	if( inst.type == "takeOff"){
		AR->takeOff();
	
	}else if( inst.type == "land"){
		AR->land();
	
	}else if( inst.type == "left"){
		AR->left(stod(inst.arg1));
		
	}else if( inst.type == "right"){
		AR->right(stod(inst.arg1));
		
	}else if( inst.type == "forward"){
		AR->forward(stod(inst.arg1));
	
	 }else if( inst.type == "backward"){
		AR->backward(stod(inst.arg1));
	
	}else if( inst.type == "up"){
		AR->up(stod(inst.arg1));
	
	}else if( inst.type == "down"){
		AR->down(stod(inst.arg1));
	
	}else if( inst.type == "rotRight"){
		AR->rotRight(stod(inst.arg1));
	
	}else if( inst.type == "rotLeft"){
		AR->rotLeft(stod(inst.arg1));
	
	}else if( inst.type == "wait"){
		AR->tempo(stod(inst.arg1));
	
	}else if( inst.type == "goToWP"){
		Waypoint WP = findWaypoint(WPtab, inst.arg1);
		double x = WP.x-AR->x;
		double y = WP.y-AR->y;
		double alt = WP.alt-AR->alt;
		
		if(x > 0){
			AR->right(x);
		}else if(x < 0){
			AR->left(-x);
		}
		
		if(y > 0){
			AR->forward(y);
		}else if(y < 0){
			AR->backward(-y);
		}
		
		if(alt > 0){
			AR->up(alt);
		}else if(alt < 0){
			AR->down(-alt);
		}
		
	}else if( inst.type == "goTo"){
		//todo
	}
}

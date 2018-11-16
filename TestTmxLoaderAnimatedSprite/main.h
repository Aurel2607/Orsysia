#pragma once



//Legends of Meruvia - C++ / SFML 2.5.1
//Copyright / Droits d'auteur : Aurel

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

// Taille de la fenêtre : 800x600 pixels
const int SCREEN_WIDTH = 800u;
const int SCREEN_HEIGHT = 600u;

const int CAMERA_INHIBITION_WIDTH = 260;
const int CAMERA_INHIBITION_HEIGHT = 200;



enum direction{
	up=0,
	down,
	left,
	right
};

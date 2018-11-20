/*********************************************************************
Matt Marchant 2013 - 2015
SFML Tiled Map Loader - https://github.com/bjorn/tiled/wiki/TMX-Map-Format
						http://trederia.blogspot.com/2013/05/tiled-map-loader-for-sfml.html

The zlib license has been used to make this software fully compatible
with SFML. See http://www.sfml-dev.org/license.php

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented;
   you must not claim that you wrote the original software.
   If you use this software in a product, an acknowledgment
   in the product documentation would be appreciated but
   is not required.

2. Altered source versions must be plainly marked as such,
   and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
   source distribution.
*********************************************************************/

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <tmx/MapLoader.hpp>
#include <tmx/Log.hpp>
#include <sstream>
#include <cassert>


//-----------------------------------------------------------------------------
namespace
{
	sf::Vector2f getViewMovement(float dt)
	{
		sf::Vector2f movement;

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			movement.x = -1.f;
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			movement.x = 1.f;

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			movement.y = -1.f;
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			movement.y = 1.f;

		movement = Helpers::Vectors::normalize(movement) * 500.f * dt;
		return movement;
	}

	void handleWindowEvent(sf::RenderWindow& renderWindow)
	{
			sf::Event event;
			while(renderWindow.pollEvent(event))
			{
				if (event.type == sf::Event::Closed
					|| (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				{
					renderWindow.close();
				}
			}
	}

	sf::Font loadFont()
	{
		//setup fonts
		sf::Font font;
		if (!font.loadFromFile("fonts/Ubuntu-M.ttf"))
		{
			std::cout << "font not loaded for fps count" << std::endl;
			//do nothing its just a test
		}
		return font;
	}

	sf::Text getFpsText(const sf::Font& font)
	{
		sf::Text fpsText;
		fpsText.setFont(font);
		fpsText.setColor(sf::Color::White);
		fpsText.setCharacterSize(25);
		return fpsText;
	}
}
int main_Benchmark()
{
    sf::RenderWindow renderWindow(sf::VideoMode(800u, 600u), "TMX Loader");
    sf::Font font = loadFont();
    sf::Text fpsText = getFpsText(font);

	//set the debugging output mode
	tmx::setLogLevel(tmx::Logger::Info | tmx::Logger::Error);

    //create map loader and load map
    tmx::MapLoader ml("maps/");
    ml.load("desert.tmx");

    sf::Clock deltaClock, frameClock;

    const float dt = 0.01f;

    float previousUpdateTime = deltaClock.getElapsedTime().asSeconds();
    float accumulator = 0.f;

    while(renderWindow.isOpen())
    {
        handleWindowEvent(renderWindow);

        //update
        float currentTime = deltaClock.getElapsedTime().asSeconds();
        float frameTime = currentTime - previousUpdateTime;
        previousUpdateTime = currentTime;
        accumulator += frameTime;

        sf::Vector2f movement;
        while ( accumulator >= dt )
        {
            movement = getViewMovement(dt);
            accumulator -= dt;
        }

        //allow moving of view
        sf::View view = renderWindow.getView();
        view.move(movement);
        renderWindow.setView(view);

        //show fps
        float fpsCount = (1.f / frameClock.restart().asSeconds());
        fpsText.setString( "FPS: " + (std::to_string(fpsCount)));
        fpsText.move(movement);

        //draw
        renderWindow.clear();
        renderWindow.draw(ml);
        renderWindow.draw(fpsText);
        renderWindow.display();
    }

    return 0;
}

//-----------------------------------------------------------------------------

void AddObject(tmx::MapLayer& layer, const sf::Vector2f& coords)
{
	const float width = 32.f;
	const float height = 64.f;

	tmx::MapObject mo;
	mo.setPosition(coords);
	mo.addPoint(sf::Vector2f());
	mo.addPoint(sf::Vector2f(width, 0.f));
	mo.addPoint(sf::Vector2f(width, height));
	mo.addPoint(sf::Vector2f(0.f, height));
	mo.createDebugShape(sf::Color::Magenta); //must always call this after adding points
											//to properly create bounding properties.

	layer.objects.push_back(mo);
}


int main_AddRemoveObjects()
{
	sf::RenderWindow renderWindow(sf::VideoMode(800u, 600u), "TMX Loader");
	renderWindow.setVerticalSyncEnabled(true);

	//create map loader and load map
	tmx::MapLoader ml("maps/");
	ml.load("addRemove.tmx");

	sf::Clock deltaClock, frameClock;

	assert(ml.getLayers().size() > 1);
	auto& objectLayer = ml.getLayers()[1];


	//-----------------------------------//

	while(renderWindow.isOpen())
	{
		//poll input
		sf::Event event;
		while(renderWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				renderWindow.close();

			if(event.type == sf::Event::KeyReleased)
			{
				switch(event.key.code)
				{
				case sf::Keyboard::Escape:
					renderWindow.close();
					break;
				case sf::Keyboard::D:
					if(objectLayer.objects.size())
					{
						//objectLayer.objects.pop_back();
						objectLayer.objects.erase(objectLayer.objects.end() - 1);
					}
					break;
				default: break;
				}
			}

			if(event.type == sf::Event::MouseButtonReleased)
			{
				switch(event.mouseButton.button)
				{
				case sf::Mouse::Left:
					{
					sf::Vector2f mousePos = renderWindow.mapPixelToCoords(sf::Mouse::getPosition(renderWindow));
					AddObject(objectLayer, mousePos);
					}
					break;
				default: break;
				}
			}
        }

		frameClock.restart();

		//draw
		renderWindow.clear();
		renderWindow.draw(ml);
		ml.drawLayer(renderWindow, tmx::MapLayer::Debug);
		renderWindow.display();

		const float time = 1.f / frameClock.getElapsedTime().asSeconds();
		std::stringstream stream;
		stream << "Click mouse to add or D to remove. Current fps: " << time << std::endl;
		renderWindow.setTitle(stream.str());
	}

	return 0;
}

//-----------------------------------------------------------------------------

const char waterShader[] =
"#version 120\n"
"uniform sampler2D baseTexture;"
"uniform float time = 1.0;" //time in seconds

//
// Description : Array and textureless GLSL 2D/3D/4D simplex
//               noise functions.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : ijm
//     Lastmod : 20110822 (ijm)
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//               https://github.com/ashima/webgl-noise
//

"vec3 mod289(vec3 x) {"
	"return x - floor(x * (1.0 / 289.0)) * 289.0;"
"}"

"vec4 mod289(vec4 x) {"
	"return x - floor(x * (1.0 / 289.0)) * 289.0;"
"}"

"vec4 permute(vec4 x) {"
	"return mod289(((x*34.0) + 1.0)*x);"
"}"

"vec4 taylorInvSqrt(vec4 r)"
"{"
	"return 1.79284291400159 - 0.85373472095314 * r;"
"}"

"float snoise(vec3 v)"
"{"
	"const vec2  C = vec2(1.0 / 6.0, 1.0 / 3.0);"
	"const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);"

	// First corner
	"vec3 i = floor(v + dot(v, C.yyy));"
	"vec3 x0 = v - i + dot(i, C.xxx);"

	// Other corners
	"vec3 g = step(x0.yzx, x0.xyz);"
	"vec3 l = 1.0 - g;"
	"vec3 i1 = min(g.xyz, l.zxy);"
	"vec3 i2 = max(g.xyz, l.zxy);"

	//   x0 = x0 - 0.0 + 0.0 * C.xxx;
	//   x1 = x0 - i1  + 1.0 * C.xxx;
	//   x2 = x0 - i2  + 2.0 * C.xxx;
	//   x3 = x0 - 1.0 + 3.0 * C.xxx;
	"vec3 x1 = x0 - i1 + C.xxx;"
	"vec3 x2 = x0 - i2 + C.yyy;" // 2.0*C.x = 1/3 = C.y
	"vec3 x3 = x0 - D.yyy;"      // -1.0+3.0*C.x = -0.5 = -D.y

	// Permutations
	"i = mod289(i);"
	"vec4 p = permute(permute(permute("
		"i.z + vec4(0.0, i1.z, i2.z, 1.0))"
		"+ i.y + vec4(0.0, i1.y, i2.y, 1.0))"
		"+ i.x + vec4(0.0, i1.x, i2.x, 1.0));"

	// Gradients: 7x7 points over a square, mapped onto an octahedron.
	// The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
	"float n_ = 0.142857142857;" // 1.0/7.0
	"vec3  ns = n_ * D.wyz - D.xzx;"

	"vec4 j = p - 49.0 * floor(p * ns.z * ns.z);"  //  mod(p,7*7)

	"vec4 x_ = floor(j * ns.z);"
	"vec4 y_ = floor(j - 7.0 * x_);"    // mod(j,N)

	"vec4 x = x_ *ns.x + ns.yyyy;"
	"vec4 y = y_ *ns.x + ns.yyyy;"
	"vec4 h = 1.0 - abs(x) - abs(y);"

	"vec4 b0 = vec4(x.xy, y.xy);"
	"vec4 b1 = vec4(x.zw, y.zw);"

	//vec4 s0 = vec4(lessThan(b0,0.0))*2.0 - 1.0;
	//vec4 s1 = vec4(lessThan(b1,0.0))*2.0 - 1.0;
	"vec4 s0 = floor(b0)*2.0 + 1.0;"
	"vec4 s1 = floor(b1)*2.0 + 1.0;"
	"vec4 sh = -step(h, vec4(0.0));"

	"vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy;"
	"vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww;"

	"vec3 p0 = vec3(a0.xy, h.x);"
	"vec3 p1 = vec3(a0.zw, h.y);"
	"vec3 p2 = vec3(a1.xy, h.z);"
	"vec3 p3 = vec3(a1.zw, h.w);"

	//Normalise gradients
	"vec4 norm = taylorInvSqrt(vec4(dot(p0, p0), dot(p1, p1), dot(p2, p2), dot(p3, p3)));"
	"p0 *= norm.x;"
	"p1 *= norm.y;"
	"p2 *= norm.z;"
	"p3 *= norm.w;"

	// Mix final noise value
	"vec4 m = max(0.6 - vec4(dot(x0, x0), dot(x1, x1), dot(x2, x2), dot(x3, x3)), 0.0);"
	"m = m * m;"
	"return 42.0 * dot(m*m, vec4(dot(p0, x0), dot(p1, x1),"
		"dot(p2, x2), dot(p3, x3)));"
"}"


"void main(void)"
"{"
	//sin distort for ripples
	"const float sinWidth = 0.08;" //smaller is wider
	"const float sinHeight = 0.001;" //larger is taller
	"const float sinTime = 9.5;" //larger is faster (if time is input in seconds then this value hertz)

	"vec2 coord = gl_TexCoord[0].xy;"

	"float offsetX = sin(gl_FragCoord.y * sinWidth + time * sinTime) * 0.5 + 0.5;"
	"coord.x += offsetX * sinHeight / 2.0;"

	"float offsetY = sin(gl_FragCoord.x * sinWidth + time * sinTime) * 0.5 + 0.5;"
	"coord.y += offsetY * sinHeight;"

	"float highlight = snoise(vec3(gl_FragCoord.xy * 0.04, time)) * 0.1;"

	//add colour for highlight
	"vec3 colour = texture2D(baseTexture, coord).rgb;"
	"colour = clamp(colour + highlight, 0.0, 1.0);"
	"gl_FragColor = vec4(colour, 1.0);"
"}";

int main_ShaderEffects()
{
	sf::RenderWindow renderWindow(sf::VideoMode(800u, 600u), "TMX Loader");
	renderWindow.setVerticalSyncEnabled(true);

	//create map loader and load map
	tmx::MapLoader ml("maps\\");
	ml.load("shader_example.tmx");

	bool showDebug = false;
	sf::Clock frameClock, shaderClock;

	//load shader and set layer to use it
	sf::Shader waterEffect;
	waterEffect.loadFromMemory(waterShader, sf::Shader::Fragment);

	ml.setLayerShader(0u, waterEffect);

	//-----------------------------------//

	while(renderWindow.isOpen())
	{
		//poll input
		sf::Event event;
		while(renderWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				renderWindow.close();
			if(event.type == sf::Event::KeyReleased)
			{
				switch(event.key.code)
				{
				case sf::Keyboard::D:
					showDebug = !showDebug;
					break;
				default: break;
				}
			}
        }

		//update shader
		waterEffect.setParameter("time", shaderClock.getElapsedTime().asSeconds());

		//draw
		frameClock.restart();
		renderWindow.clear();
		renderWindow.draw(ml);
		if(showDebug) ml.drawLayer(renderWindow, tmx::MapLayer::Debug);//draw with debug information shown
		renderWindow.display();

		renderWindow.setTitle("Press D to Toggle debug shapes. " + std::to_string(1.f / frameClock.getElapsedTime().asSeconds()));
	}

	return 0;
}

//-----------------------------------------------------------------------------

int main_MapWithQuadTree(void)
{
	sf::RenderWindow renderWindow(sf::VideoMode(800u, 600u), "TMX Loader");

	//create map loader and load map
	tmx::MapLoader ml("maps/");
	ml.load("desert.tmx");

	//-----------------------------------//

	while(renderWindow.isOpen())
	{
		//poll input
		sf::Event event;
		while(renderWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				renderWindow.close();
        }

		//build quad tree by querying visible region
		ml.updateQuadTree(sf::FloatRect(0.f, 0.f, 800.f, 600.f));
		//get a vector of MapObjects contained in the quads intersected by query area
		sf::Vector2f mousePos = renderWindow.mapPixelToCoords(sf::Mouse::getPosition(renderWindow));
		//NOTE quad tree MUST be updated before attempting to query it
		std::vector<tmx::MapObject*> objects = ml.queryQuadTree(sf::FloatRect(mousePos.x - 10.f, mousePos.y - 10.f, 20.f, 20.f));

		//do stuff with returned objects
		std::stringstream stream;
		stream << "Query object count: " << objects.size();
		renderWindow.setTitle(stream.str());

		//draw
		renderWindow.clear();
		renderWindow.draw(ml);
		ml.drawLayer(renderWindow, tmx::MapLayer::Debug); //draw with debug information shown
		renderWindow.display();
	}

	return 0;
}


int main_IsometricWithConvertCoords(void)
{
	sf::RenderWindow renderWindow(sf::VideoMode(800u, 600u), "TMX Loader - IsometricWithConvertCoords");
	renderWindow.setVerticalSyncEnabled(true);

	//create map loader and load map
	tmx::MapLoader ml("maps/");
	ml.load("isometric_grass_and_water.tmx");

	//adjust the view to centre on map
	sf::View view = renderWindow.getView();
	view.setCenter(0.f, 300.f);
	renderWindow.setView(view);

	//to toggle debug output
	bool debug = false;

	//-----------------------------------//

	while(renderWindow.isOpen())
	{
		//poll input
		sf::Event event;
		while(renderWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				renderWindow.close();
			if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D)
				debug = !debug;
        }

		//draw map
		renderWindow.clear();
		renderWindow.draw(ml);
		if(debug)ml.drawLayer(renderWindow, tmx::MapLayer::Debug);
		renderWindow.display();

		//print mouse coords to orthographic (screen) coords and world (isometric) coords
		sf::Vector2f mousePosScreen = renderWindow.mapPixelToCoords(sf::Mouse::getPosition(renderWindow));
		sf::Vector2f mousePosWorld = ml.orthogonalToIsometric(mousePosScreen);

		std::stringstream stream;
		stream << "TMX Loader - DrawMapWithDebug - Mouse Position: "<< mousePosScreen.x << ", " << mousePosScreen.y << " World Position: " << mousePosWorld.x << ", " << mousePosWorld.y;

		renderWindow.setTitle(stream.str());
	}

	return 0;
}

//-----------------------------------------------------------------------------

int main_DrawMapWithDebug(void)
{
//	sf::RenderWindow renderWindow(sf::VideoMode(800u, 600u), "TMX Loader - DrawMapWithDebug");
	sf::RenderWindow renderWindow(sf::VideoMode(320u, 320u), "TMX Loader - DrawMapWithDebug");
	//renderWindow.setVerticalSyncEnabled(true);

	//create map loader and load map
	tmx::MapLoader ml("maps\\");
//	ml.load("desert.tmx");
//	ml.load("desert2.tmx");
//	if(!ml.load("Map2.tmx")){
	if(!ml.load("desert2.tmx")){
        std::cout << "Error while loading map" << std::endl;
	}

//	//update the quad tree once so we can see it when drawing debug
//	ml.updateQuadTree(sf::FloatRect(0.f, 0.f, 800.f, 600.f));

	bool showDebug = false;
	sf::Clock frameClock;

	//-----------------------------------//

	while(renderWindow.isOpen())
	{
		//poll input
		sf::Event event;
		while(renderWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				renderWindow.close();
			if(event.type == sf::Event::KeyReleased)
			{
				switch(event.key.code)
				{
				case sf::Keyboard::D:
					showDebug = !showDebug;
					break;
				default: break;
				}
			}
        }


//		//move objects about
//		std::vector<tmx::MapLayer>& layers = ml.getLayers();
//		// Pour toutes les layers de la map
//		for(auto& l : layers)
//		{
//			// Si la layer est de type ObjectGroup
//			if(l.type == tmx::ObjectGroup)
//			{
//				// Pour tous les objets de la layer de type ObjectGroup
//				for(auto& o : l.objects)
//				{
//					o.move(0.f, 60.f * frameClock.getElapsedTime().asSeconds());
//					if(o.getPosition().y > 600.f)
//					{
//						o.setPosition(o.getPosition().x, 0.f);
//					}
//				}
//			}
//		}

//		ml.updateQuadTree(sf::FloatRect(0.f, 0.f, 800.f, 600.f));

		//draw
		frameClock.restart();
		renderWindow.clear();
		renderWindow.draw(ml);
//		if(showDebug)
//            ml.drawLayer(renderWindow, tmx::MapLayer::All);//draw with debug information shown
		renderWindow.display();

		renderWindow.setTitle("Press D to Toggle debug shapes. " + std::to_string(1.f / frameClock.getElapsedTime().asSeconds()));
	}

	return 0;
}

//-----------------------------------------------------------------------------





int main()
{
//    return main_DrawMapWithDebug();
 //   return main_IsometricWithConvertCoords();
    return main_MapWithQuadTree();
//    return main_ShaderEffects();
//    return main_AddRemoveObjects();
//    return main_Benchmark();
}


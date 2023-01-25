#include "game.h"

static bool isInTheBox(int a, int b, int w, int h, int x, int y){

    if(x > a && x < (a + w) && y > b && y < (b + h)){

        return true;

    }

    return false;
}

Game::Game(){

    associatedConnectionSocket = ConnectionSocket();

}

void Game::initializeVisualProcess(){

    bool alreadySent = false;

    sf::RenderWindow window(sf::VideoMode(gameScreenWidth, gameScreenHeight), "Snaky Game 2000!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    bool scrollBarIsPressed = false;
    float scrollPercentage = 0;
    unsigned short sessionListHeight;

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::Texture joinButtonTexture;
    sf::Sprite joinButtonSprite;

    sf::Texture createSessionButtonTexture;
    sf::Sprite createSessionButtonSprite;

    sf::Texture sessionBarTexture;
    sf::Sprite sessionBarSprite;

    sf::Texture sessionQuitTexture;
    sf::Sprite sessionQuitSprite;

    sf::RectangleShape scrollBar;
    scrollBar.setSize(sf::Vector2f(10,20));
    scrollBar.setPosition(sf::Vector2f(500,20));
    scrollBar.setFillColor(sf::Color::Yellow);

    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if(sf::Mouse::getPosition(window).x >= 0 && sf::Mouse::getPosition(window).x <= 600 && sf::Mouse::getPosition(window).y >= 0 && sf::Mouse::getPosition(window).y <= 600){

            if (event.type == sf::Event::Closed){
                associatedConnectionSocket.setRequest("endssio");
                window.close();
            }else if(currentScreen == mainMenuScreen && sf::Mouse::isButtonPressed(sf::Mouse::Left) && isInTheBox(20,20,150,75,event.mouseButton.x,event.mouseButton.y)){

                std::cout << "mpose prewssse to join << std::endl;" << std::endl;

                associatedConnectionSocket.setRequest("siolist");


            }else if(currentScreen == mainMenuScreen && sf::Mouse::isButtonPressed(sf::Mouse::Left) && isInTheBox(190,20,150,75,event.mouseButton.x,event.mouseButton.y)){

                currentScreen = sessionWaitsSecondPlayer;

                associatedConnectionSocket.waitingForRival = true;

                associatedConnectionSocket.setRequest("crtgame");

            }else if(currentScreen == sessionMenuScreen && sf::Mouse::isButtonPressed(sf::Mouse::Left) && isInTheBox(scrollBar.getPosition().x,scrollBar.getPosition().y,10,20,sf::Mouse::getPosition(window).x,sf::Mouse::getPosition(window).y) && !scrollBarIsPressed){

                scrollBarIsPressed = true;

            }else if(currentScreen == sessionMenuScreen && sf::Mouse::isButtonPressed(sf::Mouse::Left) && scrollBarIsPressed){

                float y = sf::Mouse::getPosition(window).y;

                if(sf::Mouse::getPosition(window).y < 20){

                    y = 20;

                }else if(sf::Mouse::getPosition(window).y > 550){

                    y = 550;

                }

                scrollPercentage = ((y-20.0)/530.0);

                scrollBar.setPosition(sf::Vector2f(500,20+(530*scrollPercentage)));


            }else if(currentScreen == sessionMenuScreen && !sf::Mouse::isButtonPressed(sf::Mouse::Left) && scrollBarIsPressed){

                scrollBarIsPressed = false;

            }else if(currentScreen == sessionMenuScreen && sf::Mouse::isButtonPressed(sf::Mouse::Left) && isInTheBox(5,5,20,20,sf::Mouse::getPosition(window).x,sf::Mouse::getPosition(window).y)){

                currentScreen = mainMenuScreen;

            }else if(currentScreen == sessionWaitsSecondPlayer && sf::Mouse::isButtonPressed(sf::Mouse::Left) && isInTheBox(5,5,20,20,sf::Mouse::getPosition(window).x,sf::Mouse::getPosition(window).y)){

                associatedConnectionSocket.setRequest("endssio");

                currentScreen = mainMenuScreen;

            }

            if(currentScreen == sessionMenuScreen && !scrollBarIsPressed && sf::Mouse::isButtonPressed(sf::Mouse::Left) && !alreadySent){


                float y = sf::Mouse::getPosition(window).y;

                float mapped = y + (sessionListHeight * scrollPercentage);

                for(short i = 0; i < sessionList.size(); i++){

                    if( mapped < ((i+1)*100)+((i+1)*25)){

                            std::string temp = "joinse";

                            temp.push_back(sessionList[i]);

                            associatedConnectionSocket.setRequest(&temp[0]);

                            break;
                    }
                }

                alreadySent = true;
            }
            }

            if(alreadySent){

                alreadySent = false;

            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){

                char temp[7] = {'c','m','m','n','d','E',(unsigned char)associatedConnectionSocket.sessionId};

                associatedConnectionSocket.setRequest(temp);

            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){

                char temp[7] = {'c','m','m','n','d','W',(unsigned char)associatedConnectionSocket.sessionId};

                associatedConnectionSocket.setRequest(temp);
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){

                char temp[7] = {'c','m','m','n','d','N',(unsigned char)associatedConnectionSocket.sessionId};

                associatedConnectionSocket.setRequest(temp);
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){

                char temp[7] = {'c','m','m','n','d','S',(unsigned char)associatedConnectionSocket.sessionId};

                associatedConnectionSocket.setRequest(temp);
            }
        }

        for(unsigned short i = 0; i < associatedConnectionSocket.commandVector.size(); i++){

            if(associatedConnectionSocket.commandVector[i].getHeader() == "sessionListGot"){

                sessionList.clear();

                for(unsigned short j = 0; j < 244 ;j++){

                    sessionList.push_back((unsigned char)associatedConnectionSocket.activeSessionsBuffer[j]);

                }

                associatedConnectionSocket.commandVector.erase(associatedConnectionSocket.commandVector.begin()+i);

                currentScreen = sessionMenuScreen;

            }else if(associatedConnectionSocket.commandVector[i].getHeader() == "startSession"){

                associatedSession = Session();

                associatedConnectionSocket.gameIsOn = true;

                associatedConnectionSocket.waitingForNextTurn = false;

                currentScreen = inGameScreen;

            }else if(associatedConnectionSocket.commandVector[i].getHeader() == "turnForwarded"){

                associatedConnectionSocket.waitingForNextTurn = false;

            }else if(associatedConnectionSocket.commandVector[i].getHeader() == "gameDataIsHere"){

                associatedSession.forwardTurn(associatedConnectionSocket.commandVector[i].getData(),associatedConnectionSocket.getSocketId());

                associatedConnectionSocket.waitingForNextTurn = true;

            }else if(associatedConnectionSocket.commandVector[i].getHeader() == "endGame"){

                currentScreen = mainMenuScreen;

                associatedConnectionSocket.gameIsOn = false;

                associatedConnectionSocket.waitingForNextTurn = false;

                associatedConnectionSocket.waitingForRival = false;

            }

            //delete the entire vector here...

            associatedConnectionSocket.commandVector.clear();

        }

        window.clear();

        switch(currentScreen){

            case mainMenuScreen:

                {

                backgroundTexture.loadFromFile("assets/mainMenuImage.jpg");
                backgroundSprite.setTexture(backgroundTexture);

                window.draw(backgroundSprite);

                joinButtonTexture.loadFromFile("assets/joinButtonImage.jpg");
                joinButtonSprite.setTexture(joinButtonTexture);

                joinButtonSprite.setPosition(sf::Vector2f(20,20));

                createSessionButtonTexture.loadFromFile("assets/createGameButtonImage.jpg");
                createSessionButtonSprite.setTexture(createSessionButtonTexture);

                createSessionButtonSprite.setPosition(sf::Vector2f(200,20));

                window.draw(joinButtonSprite);

                window.draw(createSessionButtonSprite);

                break;
                }
            case sessionMenuScreen:

                {

                backgroundTexture.loadFromFile("assets/sessionMenuImage.jpg");

                backgroundSprite.setTexture(backgroundTexture);

                window.draw(backgroundSprite);

                sessionBarTexture.loadFromFile("assets/sessionBar.jpg");
                sessionBarSprite.setTexture(sessionBarTexture);

                joinButtonTexture.loadFromFile("assets/joinButtonImage.jpg");
                joinButtonSprite.setTexture(joinButtonTexture);

                sessionQuitTexture.loadFromFile("assets/quit.jpg");
                sessionQuitSprite.setTexture(sessionQuitTexture);

                sessionQuitSprite.setPosition(sf::Vector2f(5,5));

                for(unsigned short i = 0; i < sessionList.size(); i++){

                    if(sessionList[i] != 0){

                        int mappedY = (25*(i+1)+(i*100))-(scrollPercentage*sessionListHeight);

                    if(mappedY >= -100 && mappedY <= 600){

                        sessionBarSprite.setPosition(sf::Vector2f(30,mappedY));

                        joinButtonSprite.setPosition(sf::Vector2f(400,mappedY+20));

                        window.draw(sessionBarSprite);

                        window.draw(joinButtonSprite);

                    }
                }
                }


                if(sessionList.size() > 4){

                    window.draw(scrollBar);
                }

                window.draw(sessionQuitSprite);

                break;

                }
            case sessionWaitsSecondPlayer:
                {

                    backgroundTexture.loadFromFile("assets/sessionInWaitingMenuImage.jpg");
                    backgroundSprite.setTexture(backgroundTexture);

                    sessionQuitTexture.loadFromFile("assets/quit.jpg");
                    sessionQuitSprite.setTexture(sessionQuitTexture);

                    sessionQuitSprite.setPosition(sf::Vector2f(5,5));

                    window.draw(backgroundSprite);

                    window.draw(sessionQuitSprite);

                    break;
                }
            case inGameScreen:
                {
                    associatedSession.drawTable(window);

                    break;
                }

            default:
                {
                    std::cout << "else" << std::endl;
                }

        }

        window.display();
    }

}

void Game::run(){

    currentScreen = mainMenuScreen;

    associatedConnectionSocket.activateSocket();

    initializeVisualProcess();

}

void Game::manageConnection(){

}

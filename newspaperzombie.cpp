#include "newspaperzombie.h"
#include "mainwindow.h"
#include "gameobject.h"
#include <QLabel>
#include <QTimer>

NewspaperZombie::NewspaperZombie()  //Default constructor
{
    width = 46;
    height = 62;
    life = 8;
    armour = 8;
    attack = 1;
    rate = 0.5;
    speed = 0.288;
    speedIsSlower = false;
    objectType = "NewspaperZombie";
    imageUrl = ":/images/newspaper-zombie.gif"; //File location of newspaper zombie
    imageUrlWidth = 46;                         //Image width
    imageUrlHeight = 62;                        //Image height
    imageUrlAlt = ":/images/normal-zombie.gif"; //File location of the normal zombie
    imageUrlAltWidth = 40;                      //Image width
    imageUrlAltHeight = 62;                     //Image height
    //Create a new label to place the zombie gif
    gameObjectGif = new QLabel();
    //Create timer to attack based on zombie rate
    multiUseTimer = new QTimer();
    //Set as single shot
    multiUseTimer->setSingleShot(true);
    //Don't start the timer
    multiUseTimer->stop();
}

NewspaperZombie::~NewspaperZombie() //Destructor
{}

void NewspaperZombie::setupGameObject() //Setup and initialize the game object
{
    //Set the label to have a transparent background
    gameObjectGif->setAttribute(Qt::WA_TranslucentBackground,true);
    //Create a new qmovie with the zombie gif
    QMovie *zombie_gif = new QMovie(imageUrl);
    //Set the position and size of the zombie gif
    gameObjectGif->setGeometry(this->scenePos().x()+5,this->scenePos().y()+5,imageUrlWidth,imageUrlHeight);
    //Set the zombie gif to the label
    gameObjectGif->setMovie(zombie_gif);
    //Start the gif
    gameObjectGif->movie()->start();
    //Add the label to the game scene
    MainWindow::gameScene->addWidget(gameObjectGif);
}

QRectF NewspaperZombie::boundingRect() const
{
    return QRectF(0,0,width,height);
}

void NewspaperZombie::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //Loop through all plant zombies
    for (int i = 0; i < MainWindow::plantObjects.size(); i++)
    {
        //If the zombie collides with a plant
        if (this->collidesWithItem(MainWindow::plantObjects.at(i)) && !multiUseTimer->isActive())
        {
            //Stop the x velocity of the zombie
            this->setXVelocity(0);
            //Subtract health from plant object
            MainWindow::plantObjects.at(i)->setLife(MainWindow::plantObjects.at(i)->getLife() - attack);
            //Check life of plant
            if (MainWindow::plantObjects.at(i)->getLife() <= 0)
            {
                this->setXVelocity(speed);
                break;
            }
            //Set the X velocity
            this->setXVelocity(0.5);
            //Start the timer to attack the zombie
            multiUseTimer->start(rate * 1000);
            break;
        }
    }
    //Check timer
    if (multiUseTimer->isActive() && multiUseTimer->remainingTime() <= (rate / 2 * 1000))
    {
        this->setXVelocity(-0.5);
    }
    //Set the position of the zombie gif to match the position of the game object
    gameObjectGif->setGeometry(this->scenePos().x()+5,this->scenePos().y()+5,imageUrlWidth,imageUrlHeight);
    //Check if the zombie armour is less than or equal to zero
    if (this->getArmour() <= 0 /*Need something else here*/)
    {
        //Create a new qmovie with the zombie gif
        QMovie *zombie_gif = new QMovie(imageUrlAlt);
        //Set the position and size of the zombie gif
        gameObjectGif->setGeometry(this->scenePos().x()+5,this->scenePos().y()+5,imageUrlAltWidth,imageUrlAltHeight);
        //Set the zombie gif to the label
        gameObjectGif->setMovie(zombie_gif);
        //Start the gif
        zombie_gif->start();
        //Set the zombie speed to a lower number
        this->setXVelocity(0.15);
    }
    //Check if the zombies health is less than or equal to zero
    if (this->getLife() <= 0)
    {
        //Delete the game label holding the gif
        delete gameObjectGif;
        gameObjectGif = 0;
        //Remove the game object from the game scene
        MainWindow::gameScene->removeItem(this);
        //Remove the game object from the zombie list
        for (int i = 0; i < MainWindow::zombieObjects.size(); i++)
        {
            if (this == MainWindow::zombieObjects.at(i))
            {
                MainWindow::zombieObjects.removeAt(i);
                break;
            }
        }
    }
    //Set the game object to be transparent
    painter->setOpacity(0);
    //Set the size of the game object
    painter->drawRect(boundingRect());
}

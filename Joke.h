#ifndef Joke_H
#define Joke_H

#include "Arduino.h"
#include "U8g2lib.h"

#include "DirectionalPad.h"

#include <string>
#include <vector>

class Joke
{
private:
    U8G2_SSD1312_128X64_NONAME_F_SW_I2C *u8g2;

    const int randomPin = D0;
    DirectionalPad *dPad;
    int *dPadArr;
    int upButton = 0;
    int rightButton = 0;
    int downButton = 0;
    int leftButton = 0;

    int lineStart = 15; // default starting value
    int linePos = lineStart;
    int lineSpacing = 10;

    // I'd rather put this in a txt file, but just having them in the class makes life simpler
    std::vector<const char *> jokeList = { // u8g2 expects text as const char* which is dumb
        "Why did Adele cross the road? To say hello from the other side.",
        "What kind of concert only costs 45 cents? A 50 Cent concert featuring Nickelback.",
        "What did the grape say when it got crushed? Nothing, it just let out a little wine.",
        "I want to be cremated as it is my last hope for a smoking hot body.",
        "Time flies like an arrow. Fruit flies like a banana.",
        "To the guy who invented zero, thanks for nothing.",
        "I had a crazy dream last night! I was swimming in an ocean of orange soda. Turns out it was just a Fanta sea.",
        "A crazy wife says to her husband that moose are falling from the sky. The husband says, it's reindeer.",
        "Ladies, if he can't appreciate your fruit jokes, you need to let that mango.",
        "Geology rocks but Geography is where it's at!",
        "What was Forrest Gump's email password? 1forrest1",
        "Did you hear about the restaurant on the moon? I heard the food was good but it had no atmosphere.",
        "Can February March? No, but April May.",
        "Need an ark to save two of every animal? I noah guy.",
        "I don't trust stairs because they're always up to something.",
        "Smaller babies may be delivered by stork but the heavier ones need a crane.",
        "My grandpa has the heart of the lion and a lifetime ban from the zoo.",
        "Why was Dumbo sad? He felt irrelephant.",
        "A man sued an airline company after it lost his luggage. Sadly, he lost his case.",
        "I lost my mood ring and I don't know how to feel about it!",
        "So what if I don't know what apocalypse means? It's not the end of the world!",
        "My friend drove his expensive car into a tree and found out how his Mercedes bends.",
        "Becoming a vegetarian is one big missed steak.",
        "I was wondering why the ball was getting bigger. Then it hit me.",
        "Some aquatic mammals at the zoo escaped. It was otter chaos!",
        "Never trust an atom, they make up everything!",
        "Waking up this morning was an eye-opening experience.",
        "Long fairy tales have a tendency to dragon.",
        "What do you use to cut a Roman Emperor's hair? Ceasers.",
        "The Middle Ages were called the Dark Ages because there were too many knights.",
        "My sister bet that I couldn't build a car out of spaghetti. You should've seen her face when I drove pasta.",
        "I made a pun about the wind but it blows.",
        "Never discuss infinity with a mathematician, they can go on about it forever.",
        "I knew a guy who collected candy canes, they were all in mint condition.",
        "My wife tried to apply at the post office but they wouldn't letter. They said only mails work here.",
        "My friend's bakery burned down last night. Now his business is toast.",
        "Getting the ability to fly would be so uplifting.",
        "It's hard to explain puns to kleptomaniacs because they always take things literally.",
        "I can't believe I got fired from the calendar factory. All I did was take a day off!",
        "England doesn't have a kidney bank, but it does have a Liverpool.",
        "What do you call the wife of a hippie? A Mississippi.",
        "A cross-eyed teacher couldn't control his pupils.",
        "She had a photographic memory, but never developed it.",
        "I wasn't originally going to get a brain transplant, but then I changed my mind.",
        "There was a kidnapping at school yesterday. Don't worry, though - he woke up!",
        "What do you get when you mix alcohol and literature? Tequila mockingbird.",
        "What washes up on tiny beaches? Microwaves.",
        "I hate how funerals are always at 9 a.m. I'm not really a mourning person.",
        "The guy who invented the door knocker got a no-bell prize.",
        "German sausage jokes are just the wurst.",
        "What do you call an alligator in a vest? An investigator.",
        "What do you call the ghost of a chicken? A poultry-geist.",
        "How does Moses make coffee? Hebrews it.",
        "Sure, I drink brake fluid. But I can stop anytime!",
        "What do you call a man with no arms and no legs stuffed in your mailbox? Bill.",
        "Somebody stole all my lamps. I couldn't be more de-lighted!",
        "I bought a boat because it was for sail.",
        "I'm reading a book about anti-gravity. It's impossible to put down!",
        "How did the picture end up in jail? It was framed!",
        "My ex-wife still misses me. But her aim is starting to improve!",
        "Coffee has a rough time in our house. It gets mugged every single morning!",
        "Why was the cookie sad? Because his mom was a wafer long!",
        "What's the difference between a hippo and a zippo? One is really heavy and the other is a little lighter!",
        "What did the sushi say to the bee? Wasabee!",
        "Why was the baby ant confused? Because all his uncles were ants!",
        "I just found out that I'm color blind. The news came completely out of the green!",
        "Why didn't the cat go to the vet? He was feline fine!",
        "Who is the penguin's favorite Aunt? Aunt-Arctica!",
        "What should a lawyer always wear to a court? A good lawsuit!",
        "The quickest way to make antifreeze? Just steal her blanket!",
        "How do you make a good egg-roll? You push it down a hill!",
        "Apple is designing a new automatic car. But they're having trouble installing Windows!",
        "I've started sleeping in our fireplace. Now I sleep like a log!",
        "That baseball player was such a bad sport. He stole third base and then just went home!",
        "Did you hear about the guy who got hit in the head with a can of soda? He was lucky it was a soft drink!",
        "The past, the present, and the future walk into a bar. It was tense!",
        "You really shouldn't be intimidated by advanced math… it's easy as pi!",
        "What did the hamburger name its baby? Patty! One lung said to another, “we be-lung together!”",
        "I asked a Frenchman if he played video games. He said Wii.",
        "Why are frogs so happy? They eat whatever bugs them.",
        "What did syrup to the waffle? I love you a waffle lot!",
        "My wife refuses to go to a nude beach with me. I think she's just being clothes-minded!",
        "Did you hear about that cheese factory that exploded in France? There was nothing left but de Brie!",
        "I'm no cheetah, you're lion!",
        "What did the ranch say when somebody opened the refrigerator? \"Hey, close the door! I'm dressing!\"",
        "I wanted to take pictures of the fog this morning but I mist my chance. I guess I could dew it tomorrow!",
        "What do you call a girl with one leg that's shorter than the other? Ilene.",
        "Towels can't tell jokes. They have a dry sense of humor.",
        "What did the buffalo say to his son? Bison.",
        "Why should you never trust a train? They have loco motives.",
        "A cabbage and celery walk into a bar and the cabbage gets served first because he was a head.",
        "What's America's favorite soda? Mini soda.",
        "What does a clock do when it's hungry? It goes back for seconds."}; // This is max length to fit on screen

public:
    Joke(U8G2_SSD1312_128X64_NONAME_F_SW_I2C *u8g2Obj);
    void parseDPad();
    const char *processString(arduino::String currentJoke);
    void writeJokeToScreen(const char *currentJoke);
};

Joke::Joke(U8G2_SSD1312_128X64_NONAME_F_SW_I2C *u8g2Obj)
{
    u8g2 = u8g2Obj;
    u8g2->clearBuffer();
    u8g2->setFont(u8g2_font_profont10_tf);
    dPad = new DirectionalPad();
    randomSeed(analogRead(randomPin));
    int randomIndex = random(0, jokeList.size());
    u8g2->drawStr(10, 10, jokeList[randomIndex]);
    while (true)
    {
        parseDPad();
        delay(100); // prevent button freaking out
        if (upButton || rightButton || downButton || leftButton)
        {
            u8g2->clearBuffer();
            randomSeed(analogRead(randomPin));
            randomIndex = random(0, jokeList.size());
            const char *currentJoke = jokeList[randomIndex];
            writeJokeToScreen(currentJoke);
            u8g2->drawRFrame(0, 0, 128, 64, 4); // draw border rounded-rectangle
            u8g2->sendBuffer();
        }
    }
}

void Joke::parseDPad()
{
    dPadArr = dPad->read();
    upButton = dPadArr[0];
    rightButton = dPadArr[1];
    downButton = dPadArr[2];
    leftButton = dPadArr[3];
}

const char *Joke::processString(arduino::String currentJoke)
{
    arduino::String temp = currentJoke;
    temp = temp.substring(0, 22);
    return temp.c_str();
}

void Joke::writeJokeToScreen(const char *currentJoke)
{
    if (strlen(currentJoke) > 22) // 22 characters is just about the edge of the screen
    {
        const char *processedStr = processString(currentJoke);
        u8g2->drawStr(10, linePos, processedStr);
        arduino::String temp = currentJoke;
        temp = temp.substring(22, strlen(currentJoke));
        const char *remainingStr = temp.c_str();
        linePos += lineSpacing;
        writeJokeToScreen(remainingStr);
        return;
    }
    else
    {
        u8g2->drawStr(10, linePos, currentJoke);
        linePos = lineStart; // reset the linePos (if we hit this else, we should be done)
        return;
    }
}

#endif
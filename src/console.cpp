//
//  console.cpp
//  digital
//
//  Created by Bryce on 11/21/16.
//
//

#include "console.hpp"
#include "entity.hpp"

static const string prompt = "$>";

Console::Console()
:TextLog("CMD"),
cursor()
{
    blinkTimer.start();
    TextLog::append(prompt);
    cursor.x = getFont()->measureString(prompt).x + 10;
}

bool Console::onKeydown(KeyEvent event)
{
    blinkTimer.start();
    string add(1, event.getChar());
    
    switch (event.getCode()) {
        case KeyEvent::KEY_RETURN:
            execute();
            cursor.y += lineSize;
            append("\n"+prompt);
            command.clear();
            break;
        case KeyEvent::KEY_BACKSPACE:
            if(command.empty())
                return false;
            command.pop_back();
            append("\b");
            break;
        default:
            append(add);
            break;
    }
    cursor.x = getFont()->measureString(prompt+command).x + 10;
    return false;
}

void Console::execute()
{
    
}

void Console::append(string text)
{
    TextLog::append(text);
    command.append(text);
    cursor.x = getFont()->measureString(prompt+command).x + 10;
}

void Console::draw()
{
    TextLog::draw();
    
    ivec2 cursorSize = ivec2(10, -20);
    if(blinkTimer.getSeconds() < 0.5) {
        gl::color(0, 0, 0);
        gl::drawSolidRect(Rectf(cursor, cursor+cursorSize));
    }
    if(blinkTimer.getSeconds() > 1) {
        blinkTimer.start();
    }
}

bool Console::onAccept(MouseEvent event, shared_ptr<Entity> e)
{
    if(contains(event.getPos())) {
        append(e->getName());
    }
    return false;
}
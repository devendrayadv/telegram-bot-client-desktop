#include "catch.hpp"

#include "intro/intro_bot_token.h"
#include "lang/lang_keys.h"

#include <QSignalSpy>
#include <QTest>

TEST_CASE("Bot Token Validation", "[bot][auth]") {
    // Mock objects
    QWidget parent;
    auto step = new Intro::BotTokenStep(&parent);
    
    SECTION("Valid token format") {
        const auto token = QString("1234567890:ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghi");
        REQUIRE(step->validateToken(token) == true);
    }
    
    SECTION("Invalid token - too short") {
        const auto token = QString("123:ABC");
        REQUIRE(step->validateToken(token) == false);
    }
    
    SECTION("Invalid token - missing colon") {
        const auto token = QString("1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghi");
        REQUIRE(step->validateToken(token) == false);
    }
    
    SECTION("Invalid token - invalid characters") {
        const auto token = QString("1234567890:ABCDEFGHIJKLMNOPQRSTUVWXYZ$%^&*()");
        REQUIRE(step->validateToken(token) == false);
    }
    
    SECTION("Error message for invalid token") {
        const auto token = QString("invalid-token");
        QSignalSpy spy(step, SIGNAL(errorOccurred(QString)));
        
        step->submit(token);
        REQUIRE(spy.count() == 1);
        QList<QVariant> arguments = spy.takeFirst();
        REQUIRE(arguments.at(0).toString() == tr::lng_bot_token_invalid(tr::now));
    }
    
    SECTION("Token submission") {
        const auto token = QString("1234567890:ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghi");
        QSignalSpy spy(step, SIGNAL(submitted(QString)));
        
        step->submit(token);
        REQUIRE(spy.count() == 1);
        QList<QVariant> arguments = spy.takeFirst();
        REQUIRE(arguments.at(0).toString() == token);
    }
    
    delete step;
}

TEST_CASE("Bot Token UI Interaction", "[bot][ui]") {
    // Mock objects
    QWidget parent;
    auto step = new Intro::BotTokenStep(&parent);
    
    SECTION("Error visibility") {
        REQUIRE(step->isErrorVisible() == false);
        
        step->showError(tr::lng_bot_token_invalid(tr::now));
        REQUIRE(step->isErrorVisible() == true);
        
        step->hideError();
        REQUIRE(step->isErrorVisible() == false);
    }
    
    SECTION("Submit button state") {
        // Initially disabled when empty
        REQUIRE(step->isSubmitEnabled() == false);
        
        // Enabled with valid input
        step->setTokenText("1234567890:ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghi");
        REQUIRE(step->isSubmitEnabled() == true);
        
        // Disabled with invalid input
        step->setTokenText("invalid");
        REQUIRE(step->isSubmitEnabled() == false);
    }
    
    delete step;
} 
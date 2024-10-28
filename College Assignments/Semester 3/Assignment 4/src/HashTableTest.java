import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

public class HashTableTest {

    HashTable quotes = new HashTable();

    @BeforeEach
    public void initialize() {

        quotes.put("Henry Ford", "Thinking is the hardest work there is, which is probably the reason why so few engage in it.");
        quotes.put("Albert Camus", "Don't walk behind me; I may not lead. Don't walk in front of me; I may not follow. Just walk beside me and be my friend.");
        quotes.put("Clint Eastwood", "I have a very strict gun control policy: if there's a gun around, I want to be in control of it.");
        quotes.put("Napoleon Bonaparte", "You must not fight too often with one enemy, or you will teach him all your art of war.");
        quotes.put("Helen Keller", "Walking with a friend in the dark is better than walking alone in the light.");
        quotes.put("Voltaire", "It is dangerous to be right in matters on which the established authorities are wrong.");
        quotes.put("Malcolm X", "Education is the passport to the future, for tomorrow belongs to those who prepare for it today.");
    }


    @Test
    public void containsTest1() {
        Assertions.assertTrue(quotes.containsKey("Malcolm X"));
    }

    @Test
    public void containsTest2() {
        Assertions.assertFalse(quotes.containsKey("Malcolm Gladwell"));
    }

    @Test
    public void getTest1() {
        Assertions.assertEquals("Walking with a friend in the dark is better than walking alone in the light.",
                quotes.get("Helen Keller"));
    }

    @Test
    public void getTest2() {
        Assertions.assertNull(quotes.get("Aristotle"));
    }

    @Test
    public void putTest1() {
        Assertions.assertTrue(quotes.put("Socrates", "The only true wisdom is in knowing you know nothing."));
    }

    @Test
    public void putTest2() {
        Assertions.assertFalse(quotes.put("Malcolm X", "If you have no critics you'll likely have no success."));
    }

    @Test
    public void massPutTest(){
        quotes.put("Henry Ford nah", "Thinking is the hardest work there is, which is probably why we shouldn't do that the reason why so few engage in it.");
        quotes.put("Albert Camus nah", "Don't walk behind me; I may not lead. Don't walk in front of me is probably why we shouldn't do that ; I may not follow. Just walk beside me and be my friend.");
        quotes.put("Clint Eastwood nah", "I have a very strict gun control policy: if there's a gun around is probably why we shouldn't do that , I want to be in control of it.");
        quotes.put("Napoleon Bonaparte nah", "You must not fight too often with one enemy, or you will t is probably why we shouldn't do that each him all your art of war.");
        quotes.put("Helen Keller nah", "Walking with a friend in the dark is better than walking alone in the light.");
        quotes.put("Voltaire nah", "It is dangerous to be right in matters on which the established autho is probably why we shouldn't do that rities are wrong.");
        quotes.put("Malcolm X nah", "Education is the passport to the future, for tomorrow belongs to tho is probably why we shouldn't do that se who prepare for it today.");
        quotes.put("coke Henry Ford nah", "Thinking is the hardest work there is, which is probably why we shouldn't do that the reason why so few engage in it.");
        quotes.put("coke Albert Camus nah", "Don't walk behind me; I may not lead. Don't walk in front of me is probably why we shouldn't do that ; I may not follow. Just walk beside me and be my friend.");
        quotes.put("coke Clint Eastwood nah", "I have a very strict gun control policy: if there's a gun around is probably why we shouldn't do that , I want to be in control of it.");
        quotes.put("coke Napoleon Bonaparte nah", "You must not fight too often with one enemy, or you will t is probably why we shouldn't do that each him all your art of war.");
        quotes.put("coke Helen Keller nah", "Walking with a friend in the dark is better than walking alone in the light.");
        quotes.put("coke Voltaire nah", "It is dangerous to be right in matters on which the established autho is probably why we shouldn't do that rities are wrong.");
        quotes.put("cokewcse Malcolm X nah", "Education is the passport to the future, for tomorrow belongs to tho is probably why we shouldn't do that se who prepare for it today.");
        quotes.put("Henurjry Ford nah", "Thinking is the hardest work there is, which is probably why we shouldn't do that the reason why so few engage in it.");
        quotes.put("Albejurt Camus nah", "Don't walk behind me; I may not lead. Don't walk in front of me is probably why we shouldn't do that ; I may not follow. Just walk beside me and be my friend.");
        quotes.put("Clinjtbgfyrjht Eastwood nah", "I have a very strict gun control policy: if there's a gun around is probably why we shouldn't do that , I want to be in control of it.");
        quotes.put("Napolewerton Bonaparte nah", "You must not fight too often with one enemy, or you will t is probably why we shouldn't do that each him all your art of war.");
        quotes.put("Helendfsg Keller nah", "Walking with a friend in the dark is better than walking alone in the light.");
        quotes.put("Voltaiefsdsvrtwredfsg nah", "It is dangerous to be right in matters on which the established autho is probably why we shouldn't do that rities are wrong.");
        quotes.put("Malcolfdsgmfsdg X nah", "Education is the passport to the future, for tomorrow belongs to tho is probably why we shouldn't do that se who prepare for it today.");
        quotes.put("cofdsgke Henry Ford nah", "Thinking is the hardest work there is, which is probably why we shouldn't do that the reason why so few engage in it.");
        quotes.put("cosfdgke Albert Camus nah", "Don't walk behind me; I may not lead. Don't walk in front of me is probably why we shouldn't do that ; I may not follow. Just walk beside me and be my friend.");
        quotes.put("coksdfge Clint Eastwood nah", "I have a very strict gun control policy: if there's a gun around is probably why we shouldn't do that , I want to be in control of it.");
        quotes.put("coske Napoleon Bonaparte nah", "You must not fight too often with one enemy, or you will t is probably why we shouldn't do that each him all your art of war.");
        quotes.put("cosdke Helen Keller nah", "Walking with a friend in the dark is better than walking alone in the light.");
        quotes.put("cokme Voltaire nah", "It is dangerous to be right in matters on which the established autho is probably why we shouldn't do that rities are wrong.");
        quotes.put("cokne Malcolm X nah", "Education is the passport to the future, for tomorrow belongs to tho is probably why we shouldn't do that se who prepare for it today.");
        quotes.put("Hebnry Ford nah", "Thinking is the hardest work there is, which is probably why we shouldn't do that the reason why so few engage in it.");
        quotes.put("Albvert Camus nah", "Don't walk behind me; I may not lead. Don't walk in front of me is probably why we shouldn't do that ; I may not follow. Just walk beside me and be my friend.");
        quotes.put("Clcint Eastwood nah", "I have a very strict gun control policy: if there's a gun around is probably why we shouldn't do that , I want to be in control of it.");
        quotes.put("Napxoleon Bonaparte nah", "You must not fight too often with one enemy, or you will t is probably why we shouldn't do that each him all your art of war.");
        quotes.put("Hezlen Keller nah", "Walking with a friend in the dark is better than walking alone in the light.");
        quotes.put("Volltaire nah", "It is dangerous to be right in matters on which the established autho is probably why we shouldn't do that rities are wrong.");
        quotes.put("Maklcolm X nah", "Education is the passport to the future, for tomorrow belongs to tho is probably why we shouldn't do that se who prepare for it today.");
        quotes.put("cjoke Henry Ford nah", "Thinking is the hardest work there is, which is probably why we shouldn't do that the reason why so few engage in it.");
        quotes.put("choke Albert Camus nah", "Don't walk behind me; I may not lead. Don't walk in front of me is probably why we shouldn't do that ; I may not follow. Just walk beside me and be my friend.");
        quotes.put("cgoke Clint Eastwood nah", "I have a very strict gun control policy: if there's a gun around is probably why we shouldn't do that , I want to be in control of it.");
        quotes.put("cfoke Napoleon Bonaparte nah", "You must not fight too often with one enemy, or you will t is probably why we shouldn't do that each him all your art of war.");
        quotes.put("cdoke Helen Keller nah", "Walking with a friend in the dark is better than walking alone in the light.");
        quotes.put("csoke Voltaire nah", "It is dangerous to be right in matters on which the established autho is probably why we shouldn't do that rities are wrong.");
        quotes.put("caoke Malcolm X nah", "Education is the passport to the future, for tomorrow belongs to tho is probably why we shouldn't do that se who prepare for it today.");
        quotes.put("Hsdaenry Ford nah", "Thinking is the hardest work there is, which is probably why we shouldn't do that the reason why so few engage in it.");
        quotes.put("Albdsafert Camus nah", "Don't walk behind me; I may not lead. Don't walk in front of me is probably why we shouldn't do that ; I may not follow. Just walk beside me and be my friend.");
        quotes.put("Clint Eastfdsgwood nah", "I have a very strict gun control policy: if there's a gun around is probably why we shouldn't do that , I want to be in control of it.");
        quotes.put("Napdsafoleon Bonaparte nah", "You must not fight too often with one enemy, or you will t is probably why we shouldn't do that each him all your art of war.");
        quotes.put("Heldsafen Keller nah", "Walking with a friend in the dark is better than walking alone in the light.");
        quotes.put("Voltafasdire nah", "It is dangerous to be right in matters on which the established autho is probably why we shouldn't do that rities are wrong.");
        quotes.put("Malcoasdflm X nah", "Education is the passport to the future, for tomorrow belongs to tho is probably why we shouldn't do that se who prepare for it today.");
        quotes.put("coke Hasdfenry Ford nah", "Thinking is the hardest work there is, which is probably why we shouldn't do that the reason why so few engage in it.");
        quotes.put("coke Alsadfbert Camus nah", "Don't walk behind me; I may not lead. Don't walk in front of me is probably why we shouldn't do that ; I may not follow. Just walk beside me and be my friend.");
        quotes.put("coke Chfdhtlint Eastwood nah", "I have a very strict gun control policy: if there's a gun around is probably why we shouldn't do that , I want to be in control of it.");
        quotes.put("coke Napolesdafon Bonaparte nah", "You must not fight too often with one enemy, or you will t is probably why we shouldn't do that each him all your art of war.");
        quotes.put("coke Helenwer Keller nah", "Walking with a friend in the dark is better than walking alone in the light.");
        quotes.put("coke Voljhgtaire nah", "It is dangerous to be right in matters on which the established autho is probably why we shouldn't do that rities are wrong.");
        quotes.put("cokewcseqwer Malcolm X nah", "Education is the passport to the future, for tomorrow belongs to tho is probably why we shouldn't do that se who prepare for it today.");
        quotes.put("Henurjry Fordxcvxc nah", "Thinking is the hardest work there is, which is probably why we shouldn't do that the reason why so few engage in it.");
        quotes.put("Albejurt Casfamus nah", "Don't walk behind me; I may not lead. Don't walk in front of me is probably why we shouldn't do that ; I may not follow. Just walk beside me and be my friend.");
        quotes.put("Clinjtbgfyrjfgbnbbcvjht Eastwood nah", "I have a very strict gun control policy: if there's a gun around is probably why we shouldn't do that , I want to be in control of it.");
        quotes.put("Napolewertertweron Bonaparte nah", "You must not fight too often with one enemy, or you will t is probably why we shouldn't do that each him all your art of war.");
        quotes.put("Helendfsg Kelcsdfgler nah", "Walking with a friend in the dark is better than walking alone in the light.");
        quotes.put("Voltaiefsdsreerwvrtwredfsg nah", "It is dangerous to be right in matters on which the established autho is probably why we shouldn't do that rities are wrong.");
        quotes.put("Malcolfdsgmfsdg X n3445ah", "Education is the passport to the future, for tomorrow belongs to tho is probably why we shouldn't do that se who prepare for it today.");
        quotes.put("cofdsgke Henry F234ord nah", "Thinking is the hardest work there is, which is probably why we shouldn't do that the reason why so few engage in it.");
        quotes.put("cosfdgke Albert 643Camus nah", "Don't walk behind me; I may not lead. Don't walk in front of me is probably why we shouldn't do that ; I may not follow. Just walk beside me and be my friend.");
        quotes.put("coksdfge Clint E7564astwood nah", "I have a very strict gun control policy: if there's a gun around is probably why we shouldn't do that , I want to be in control of it.");
        quotes.put("coske Napoleon Bo8654naparte nah", "You must not fight too often with one enemy, or you will t is probably why we shouldn't do that each him all your art of war.");
        quotes.put("cosdke Helen234 Keller nah", "Walking with a friend in the dark is better than walking alone in the light.");
        quotes.put("cokme Volt7658aire nah", "It is dangerous to be right in matters on which the established autho is probably why we shouldn't do that rities are wrong.");
        quotes.put("cokne Malcol3245m X nah", "Education is the passport to the future, for tomorrow belongs to tho is probably why we shouldn't do that se who prepare for it today.");
        quotes.put("Hebnry Ford 879nah", "Thinking is the hardest work there is, which is probably why we shouldn't do that the reason why so few engage in it.");
        quotes.put("Albvert Camu234s nah", "Don't walk behind me; I may not lead. Don't walk in front of me is probably why we shouldn't do that ; I may not follow. Just walk beside me and be my friend.");
        quotes.put("Clcint Eastwoo7567d nah", "I have a very strict gun control policy: if there's a gun around is probably why we shouldn't do that , I want to be in control of it.");
        quotes.put("Napxoleon Bonap754arte nah", "You must not fight too often with one enemy, or you will t is probably why we shouldn't do that each him all your art of war.");
        quotes.put("Hezlen Keller na6453h", "Walking with a friend in the dark is better than walking alone in the light.");
        quotes.put("Volltaire nah43643", "It is dangerous to be right in matters on which the established autho is probably why we shouldn't do that rities are wrong.");
        quotes.put("Mak3456lcolm X nah", "Education is the passport to the future, for tomorrow belongs to tho is probably why we shouldn't do that se who prepare for it today.");
        quotes.put("cjoke 75683Henry Ford nah", "Thinking is the hardest work there is, which is probably why we shouldn't do that the reason why so few engage in it.");
        quotes.put("choke Albe3456rt Camus nah", "Don't walk behind me; I may not lead. Don't walk in front of me is probably why we shouldn't do that ; I may not follow. Just walk beside me and be my friend.");
        quotes.put("cgoke Clint Ea4536stwood nah", "I have a very strict gun control policy: if there's a gun around is probably why we shouldn't do that , I want to be in control of it.");
        quotes.put("cfoke Napoleo345n Bonaparte nah", "You must not fight too often with one enemy, or you will t is probably why we shouldn't do that each him all your art of war.");
        quotes.put("cdoke Hele84573n Keller nah", "Walking with a friend in the dark is better than walking alone in the light.");
        quotes.put("csoke Voltai7357re nah", "It is dangerous to be right in matters on which the established autho is probably why we shouldn't do that rities are wrong.");
        quotes.put("caoke Mal222colm X nah", "Education is the passport to the future, for tomorrow belongs to tho is probably why we shouldn't do that se who prepare for it today.");
        Assertions.assertFalse(quotes.containsKey("Malcolm Gladwell"));
        Assertions.assertTrue(quotes.containsKey("Malcolm X"));
        Assertions.assertNull(quotes.get("Aristotle"));
        Assertions.assertEquals("Walking with a friend in the dark is better than walking alone in the light.", quotes.get("Helen Keller"));
    }
}
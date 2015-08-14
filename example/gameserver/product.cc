#include "product.h"

std::map<std::string, std::string> products_map ={
  {"ULUS10511", "Ace Combat X2 - Joint Assault"},
  {"NPUH10023", "Armored Core 3 Portable"},
  {"ULUS10516", "Blood Bowl"},
  {"ULUS10437", "Dissidia Final Fantasy"},
  {"ULUS10566", "Dissidia 012 Duodecim Final Fantasy"},
  {"ULUS10177", "Dungeon Siege - Throne of Agony"},
  {"UCUS98740", "Fat Princess - Fistful of Cake"},
  {"NPUG80086", "flOw"},
  {"ULJS00237", "God Eater"},
  {"ULUS10563", "God Eater Burst"},
  {"ULJM05709", "K-ON! Houkago Live"},
  {"ULUS10290", "Metal Gear Solid - Portable Ops +"},
  {"ULUS10202", "Metal Gear Solid - Portable Ops"},
  {"ULUS10513", "Split Second - Velocity"},
  {"UCUS98615", "SOCOM - Fireteam Bravo"},
  {"UCUS98645", "SOCOM - Fireteam Bravo 2"},
  {"UCUS98716", "SOCOM - Fireteam Bravo 3"},
  {"ULUS10457", "Soul Calibur - Broken Destiny"},
  {"NPJH50045", "Metal Gear Solid - Peace Walker"},
  {"ULUS10062", "Street Fighter Alpha 3 MAX"},
  {"ULUS10466", "Tekken 6"},
  {"ULUS10383", "Midnight Club - LA Remix"},
  {"UCUS98741", "Mod Nation Racers"},
  {"ULUS10084", "Monster Hunter"},
  {"ULUS10266", "Monster Hunter 2"},
  {"ULUS10391", "Monster Hunter 2G"},
  {"ULJM05800", "Monster Hunter 3"},
  {"ULUS10340", "N+"},
  {"UCUS98758", "MLB11 - The Show"},
  {"ULUS10064", "Outrun 2006 - Coast 2 Coast"},
  {"ULJM05553", "Warship Gunner 2 Portable"},
  {"ULUS10086", "Untold Legends - The Warrior's Code"},
  {"ULUS10003", "Untold Legends - Brotherhood of the Blade"},
  {"ULUS10438", "Pangya! - Fantasy Golf"},
  {"ULUS10586", "PRO Evolution Soccer 2012"},
  {"ULUS10246", "Virtua Tennis 3"},
  {"ULUS10321", "World Series of Poker 2008 - Battle for the Bracelets"},
  {"ULUS10065", "Worms Open Warfare"},
  {"ULUS10260", "Worms Open Warfare 2"},
  {"NPUH10045", "Worms Battle Islands"},
  {"UCUS98632", "Gran Turismo"},
  {"ULUS10057", "Bust A Move Deluxe"},
  {"ULUS10351", "Code Lyoko - Quest for Infinity"},
  {"ULUS10180", "M.A.C.H. - Modified Air Combat Heroes"},
  {"NPJH50583", "Conception - Please have my children!"},
  {"NPUH10020", "Strikers 1945 Plus Portable"},
  {"UCES00758", "Smash Court Tennis 3"},
  {"ULES01507", "Lord of Arcana"},
  {"ULUS10218", "Call of Duty - Roads to Victory"},
  {"ULUS10582", "Naruto Shippuden - Ultimate Ninja Impact"},
  {"ULUS10149", "Pac Man - World Rally"},
  {"UCJS10090", "Resistance - Retribution"},
  {"ULUS10194", "Shrek - Smash and Crash Racing"},
  {"ULES00670", "Rocky Balboa"},
  {"ULUS10195", "Sonic Rivals"},
  {"ULUS10210", "Ghost Rider"},
  {"ULUS10087", "Viewtiful Joe"},
  {"ULUS10508", "UFC Undisputed 2010"},
  {"UCUS98751", "Patapon 3"},
  {"ULJM05734", "Yu-Gi-Oh! 5D's Tag Force 5"},
  {"ULJM05637", "Shin Sangoku Musou - Multi Raid 2"},
  {"ULJM05940", "Yu-Gi-Oh! 5D's Tag Force 6"},
  {"NPJH50426", "Taiko no Tatsujin Portable DX"},
  {"UCUS98646", "Killzone Liberation"},
  {"ULUS10487", "LEGO Indiana Jones 2"},
  {"NPJH50107", "Gundam VS Gundam - Next Plus"},
  {"NPJH50492", "Puyo Puyo!! 20th Anniversary"},
  {"ULUS10579", "BlazBlue - Continuum Shift 2"},
  {"ULES00719", "Asphalt - Urban GT 2"},
  {"ULJS00360", "Rurouni Kenshin - Meiji Kenkaku Romantan Saisen"},
  {"ULUS10121", "Bomberman"},
  {"ULUS10537", "Dragon Ball Z - Tenkaichi Tag Team"},
  {"NPJH50520", "Pro Yakyuu Spirits 2012"},
  {"ULUS10319", "Bomberman Land"},
  {"ULUS10234", "Dragon Ball Z - Shin Budokai 2"},
  {"UCUS98732", "Patapon 2"},
  {"ULUS10376", "Need for Speed - Undercover"},
  {"ULUS10410", "Phantasy Star Portable"},
  {"ULUS10529", "Phantasy Star Portable 2"},
  {"ULJM05732", "Phantasy Star Portable 2 - Infinity"},
  {"ULUS10081", "Dragon Ball Z - Shin Budokai"},
  {"ULJM05933", "Hatsune Miku - Project Diva Extend"},
  {"ULUS10298", "Hot Pixel"},
  {"UCUS98701", "Everybody's Tennis"},
  {"ULUS10548", "TRON - Evolution"}
};
std::map<std::string, std::string> crosslinks_map = {
  {"ULES01408", "ULUS10511"},
  {"NPJH50263", "ULUS10511"},
  {"ULJM05492", "NPUH10023"},
  {"ULES00569", "ULUS10177"},
  {"ULES01519", "ULUS10563"},
  {"NPJH50352", "ULUS10563"},
  {"ULES01402", "ULUS10513"},
  {"ULJM05812", "ULUS10513"},
  {"UCES01312", "UCUS98740"},
  {"NPHG00025", "UCUS98740"},
  {"UCES00038", "UCUS98615"},
  {"UCJS10102", "UCUS98615"},
  {"UCES00543", "UCUS98645"},
  {"UCES01242", "UCUS98716"},
  {"NPJG00035", "UCUS98716"},
  {"ULES00262", "ULUS10064"},
  {"ULES01376", "ULUS10466"},
  {"NPJH50184", "ULUS10466"},
  {"ULJS00224", "ULUS10466"},
  {"ULES01298", "ULUS10457"},
  {"ULJS00202", "ULUS10457"},
  {"ULES01505", "ULUS10566"},
  {"NPJH50377", "ULUS10566"},
  {"ULES01270", "ULUS10437"},
  {"ULJM05262", "ULUS10437"},
  {"ULES01003", "ULUS10290"},
  {"ULJM05261", "ULUS10290"},
  {"ULES00645", "ULUS10202"},
  {"ULJM05193", "ULUS10202"},
  {"ULES00301", "ULUS10086"},
  {"ULJM05179", "ULUS10086"},
  {"ULKS46069", "ULUS10086"},
  {"ULES00046", "ULUS10003"},
  {"ULJM05087", "ULUS10003"},
  {"ULKS46015", "ULUS10003"},
  {"ULJM05440", "ULUS10438"},
  {"ULES01540", "ULUS10586"},
  {"ULES01541", "ULUS10586"},
  {"ULES01542", "ULUS10586"},
  {"ULAS42289", "ULUS10586"},
  {"ULKS46164", "ULUS10438"},
  {"ULES01144", "ULUS10383"},
  {"ULJS00180", "ULUS10383"},
  {"UCES01327", "UCUS98741"},
  {"UCJS10112", "UCUS98741"},
  {"UCAS40306", "UCUS98741"},
  {"ULES01213", "ULUS10391"},
  {"ULJM05500", "ULUS10391"},
  {"ULJM05066", "ULUS10084"},
  {"ULES00318", "ULUS10084"},
  {"ULJM05156", "ULUS10266"},
  {"ULES00851", "ULUS10266"},
  {"ULES01026", "ULUS10340"},
  {"ULES00763", "ULUS10246"},
  {"ULES00991", "ULUS10321"},
  {"ULES00819", "ULUS10260"},
  {"ULES00268", "ULUS10065"},
  {"NPEH00019", "NPUH10045"},
  {"ULES01230", "ULUS10516"},
  {"ULES01372", "NPJH50045"},
  {"ULUS10509", "NPJH50045"},
  {"UCES01245", "UCUS98632"},
  {"ULES00565", "ULUS10180"},
  {"ULES00618", "ULUS10194"},
  {"UCES01421", "UCUS98751"},
  {"NPJG00122", "UCUS98751"},
  {"ULES00643", "ULUS10218"},
  {"ULES01456", "ULUS10537"},
  {"NPJH50401", "ULUS10579"},
  {"ULJM05034", "ULUS10121"},
  {"ULES00469", "ULUS10121"},
  {"ULJM05316", "ULUS10121"},
  {"ULJM05181", "ULUS10319"},
  {"ULJM05319", "ULUS10319"},
  {"ULES00959", "ULUS10319"},
  {"ULJM05082", "ULUS10062"},
  {"ULES00235", "ULUS10062"},
  {"ULJM05225", "ULUS10062"},
  {"ULJS00107", "ULUS10234"},
  {"ULES00789", "ULUS10234"},
  {"UCJS10089", "UCUS98732"},
  {"PSPJ30000", "UCUS98732"},
  {"UCES01177", "UCUS98732"},
  {"UCJS18036", "UCUS98732"},
  {"ULJM05403", "ULUS10376"},
  {"ULJM05612", "ULUS10376"},
  {"ULES01145", "ULUS10376"},
  {"ULJM05309", "ULUS10410"},
  {"ULES01218", "ULUS10410"},
  {"ULJM08023", "ULUS10410"},
  {"ULJM05493", "ULUS10529"},
  {"ULJM08030", "ULUS10529"},
  {"ULES01439", "ULUS10529"},
  {"ULJS00049", "ULUS10081"},
  {"ULKS46085", "ULUS10081"},
  {"ULES00309", "ULUS10081"},
  {"NPJH50465", "ULJM05933"},
  {"ULES00642", "ULUS10298"},
  {"ULJS00250", "NPJH50107"},
  {"ULJS19048", "NPJH50107"},
  {"UCJS10101", "UCUS98701"},
  {"UCES01420", "UCUS98701"},
  {"ULES01495", "ULUS10548"},
  {"ULJS00383", "NPJH50426"},
  {"ULUS10555", "ULJM05734"},
  {"ULES01474", "ULJM05734"},
  {"ULUS10233", "ULES00670"},
  {"UCES01184", "UCJS10090"},
  {"UCUS98668", "UCJS10090"},
  {"ULJM05767", "ULES01507"},
  {"ULUS10479", "ULES01507"},
  {"ULJS00098", "UCES00758"},
  {"ULUS10269", "UCES00758"},
  {"ULES00566", "ULUS10180"},
  {"ULJM05202", "ULUS10180"}
};

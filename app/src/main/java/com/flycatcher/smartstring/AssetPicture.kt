package com.flycatcher.smartstring

enum class AssetPicture(val filename: String) {
    SMILE_GIRL_GREY("SmileGirlGrey.png"),
    SMILE_GIRL("SmileGirl.png"),
    EINSTEIN("Einstein.jpg"),
    DAVID_1300("David1300.jpg"),
    FLYCATCHER_LOGO("FlyCatcherLogo.png"),
    GREEK_WOMAN("GreekWoman.jpg"),
    GREEK_WOMAN_CROPPED("GreekWomanCropped.jpg"),
    ICON_GIFT("IconGift.png"),
    ICON_ROCKET("IconRocket.png"),
    MONA_LISA("MonaLisa.jpg"),
    MORENA_BACCARIN("MorenaBaccarin.jpg"),
    KID_DRAWING("KidDrawing.png"),
    PHILIPINE_EAGLE("PhillipineEagle.jpg"),
    PROJECTOR("SmartSketcherProjector.png"),
    SHORT_HAIR("ShortHair.jpg"),
    UNNAMED_STAR("UnnamedStar.jpg");

    fun next(): AssetPicture {
        return if (ordinal >= entries.size - 1) entries[0] else entries[ordinal+1]
    }

    fun prev(): AssetPicture {
        return if (ordinal == 0) entries[entries.size - 1] else entries[ordinal-1]
    }

}
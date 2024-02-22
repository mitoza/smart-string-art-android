package com.flycatcher.smartstring

import android.content.res.AssetManager
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.graphics.Picture
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import com.flycatcher.smartstringart.R
import com.flycatcher.smartstringart.databinding.ActivityMainBinding
import java.io.IOException
import java.io.InputStream

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding
    private lateinit var jniBridge: JniBridge

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        jniBridge = JniBridge()
        binding.mainText.text = jniBridge.showText("Hello Jni")

        binding.preview.setImageBitmap(getPicture(AssetPicture.SMILE_GIRL))

    }

    fun getPicture(picture: AssetPicture): Bitmap {
        var bitmap: Bitmap = Bitmap.createBitmap(1, 1, Bitmap.Config.RGB_565)
        var inputStream: InputStream? = null
        try {
            inputStream = assets.open(picture.filename)
            bitmap = BitmapFactory.decodeStream(inputStream)
        } catch (e: IOException) {
            e.printStackTrace()
        } finally {
            inputStream?.close()
        }
        return bitmap
    }

}
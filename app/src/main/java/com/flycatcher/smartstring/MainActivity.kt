package com.flycatcher.smartstring

import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.os.Bundle
import androidx.activity.viewModels
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.ViewModelProvider
import com.flycatcher.smartstringart.databinding.ActivityMainBinding
import java.io.IOException
import java.io.InputStream

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding
    private lateinit var jniBridge: JniBridge
    private val viewModel: MainActivityViewModel by viewModels()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        jniBridge = JniBridge()
        //binding.mainText.text = jniBridge.showText("Hello Jni")

        viewModel.currentPicture.observe(this, this::updatePictureInfo)
        binding.btnPrevPicture.setOnClickListener(viewModel::setPrevPicture)
        binding.btnNextPicture.setOnClickListener(viewModel::setNextPicture)
    }

    private fun getPicture(picture: AssetPicture): Bitmap {
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

    private fun updatePictureInfo(picture: AssetPicture) {
        binding.tvPictureName.text = picture.filename
        binding.preview.setImageBitmap(getPicture(picture))
    }

}
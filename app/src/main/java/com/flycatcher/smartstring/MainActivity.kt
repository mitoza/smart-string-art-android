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
    private val viewModel: MainActivityViewModel by viewModels()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        viewModel.getCurrentPictureLiveData().observe(this, this::updatePictureInfo)
        viewModel.getCurrentBitmapLiveData().observe(this, this::updateBitmap)

        binding.cImage.btnPrevPicture.setOnClickListener(viewModel::setPrevPicture)
        binding.cImage.btnNextPicture.setOnClickListener(viewModel::setNextPicture)
        binding.cGen.btnMakeIt.setOnClickListener(viewModel::generatePicture)
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
        binding.cImage!!.tvPictureName.text = picture.filename
        viewModel.setBitmap(getPicture(picture))
    }

    private fun updateBitmap(bitmap: Bitmap) {
        binding.mainPreview.setImageBitmap(bitmap)
    }

}
package com.flycatcher.smartstring

import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.os.Bundle
import android.view.View
import androidx.activity.viewModels
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.lifecycleScope
import com.flycatcher.smartstringart.databinding.ActivityMainBinding
import kotlinx.coroutines.*
import kotlinx.coroutines.channels.Channel
import kotlinx.coroutines.flow.collect
import java.io.IOException
import java.io.InputStream

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding
    private val viewModel: MainActivityViewModel by viewModels()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        lifecycleScope.launch {
            viewModel.bitmapFlow.collect { bitmap ->
                updateBitmap(bitmap)
            }
        }
        lifecycleScope.launch {
            viewModel.currentPicture.collect { picture ->
                updatePictureInfo(picture)
            }
        }

        binding.cImage.btnPrevPicture.setOnClickListener(this::onPrevPictureClick)
        binding.cImage.btnNextPicture.setOnClickListener(this::onNextPictureClick)
        binding.cGen.btnMakeIt.setOnClickListener(this::onGeneratePictureClick)

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
        binding.cImage.tvPictureName.text = picture.filename
        viewModel.setBitmap(getPicture(picture))
    }

    private fun updateBitmap(bitmap: Bitmap?) {
        binding.mainPreview.setImageBitmap(bitmap)
        binding.mainProgress.visibility = View.GONE
    }

    private fun onPrevPictureClick(v: View) {
        viewModel.showPrevPicture()
    }

    private fun onNextPictureClick(v: View) {
        viewModel.showNextPicture()
    }

    private fun onGeneratePictureClick(v: View) {
        val bitmap = getPicture(viewModel.currentPicture.value);
        val pins = binding.cGen.snvPins.getProgress()
        val minDistance = binding.cGen.snvMinDistance.getProgress()
        val maxLines = binding.cGen.snvMaxLines.getProgress()
        val lineWeight = binding.cGen.snvMinDistance.getProgress()
        viewModel.generatePicture(bitmap, pins, minDistance, maxLines, lineWeight)
        binding.mainProgress.visibility = View.VISIBLE
    }

}
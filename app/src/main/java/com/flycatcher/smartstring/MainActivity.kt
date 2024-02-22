package com.flycatcher.smartstring

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import com.flycatcher.smartstringart.R
import com.flycatcher.smartstringart.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding
    private lateinit var jniBridge: JniBridge

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        jniBridge = JniBridge()
        binding.mainText.text = jniBridge.showText("Hello Jni")

    }

}
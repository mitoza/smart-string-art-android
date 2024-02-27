package com.flycatcher.smartstring

import android.content.Context
import android.os.Build
import android.util.AttributeSet
import android.view.LayoutInflater
import android.widget.SeekBar
import androidx.constraintlayout.widget.ConstraintLayout
import com.flycatcher.smartstringart.R
import com.flycatcher.smartstringart.databinding.ViewSeekNumberBinding

class SeekNumberView @JvmOverloads constructor(context: Context, attrSet: AttributeSet? = null, defStyleAttr: Int = -1) :
    ConstraintLayout(context, attrSet, defStyleAttr),
    SeekBar.OnSeekBarChangeListener {

    val binding: ViewSeekNumberBinding =
        ViewSeekNumberBinding.inflate(LayoutInflater.from(context), this, true)

    var listener: OnSeekNumberProgressListener? = null

    init {
        val a = context.obtainStyledAttributes(
            attrSet,
            R.styleable.SeekNumberView,
            defStyleAttr,
            0
        )

        val min = a.getInteger(R.styleable.SeekNumberView_android_min, 0);
        val max = a.getInteger(R.styleable.SeekNumberView_android_max, 100);
        val progress = a.getInteger(R.styleable.SeekNumberView_android_progress, 100);
        val text = a.getText(R.styleable.SeekNumberView_android_text)

        a.recycle()

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            binding.venSeekBar.min = min
        }
        binding.venSeekBar.max = max
        binding.venSeekBar.progress = progress
        binding.venValue.text = progress.toString()
        binding.venTitle.text = text
        binding.venSeekBar.setOnSeekBarChangeListener(this)
    }

    fun setProgress(progress: Int) {
        binding.venSeekBar.progress = progress
    }

    fun getProgress(): Int {
        return binding.venSeekBar.progress
    }

    override fun onProgressChanged(seekBar: SeekBar, progress: Int, fromUser: Boolean) {
        binding.venValue.text = progress.toString()
        listener?.onProgressChanged(progress)
    }

    override fun onStartTrackingTouch(seekBar: SeekBar) {

    }

    override fun onStopTrackingTouch(seekBar: SeekBar) {

    }

    interface OnSeekNumberProgressListener {
        fun onProgressChanged(progress: Int)
    }

}
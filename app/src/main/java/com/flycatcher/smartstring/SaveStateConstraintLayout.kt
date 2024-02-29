package com.flycatcher.smartstring

import android.content.Context
import android.os.Build
import android.os.Build.VERSION_CODES.TIRAMISU
import android.os.Bundle
import android.os.Parcel
import android.os.Parcelable
import android.util.AttributeSet
import android.util.SparseArray
import androidx.constraintlayout.widget.ConstraintLayout
import androidx.core.view.children

open class SaveStateConstraintLayout @JvmOverloads constructor(
    context: Context,
    attrSet: AttributeSet? = null,
    defStyleAttr: Int = -1
) :
    ConstraintLayout(context, attrSet, defStyleAttr) {


    override fun dispatchSaveInstanceState(container: SparseArray<Parcelable>) {
        dispatchFreezeSelfOnly(container)
    }

    override fun dispatchRestoreInstanceState(container: SparseArray<Parcelable>) {
        dispatchThawSelfOnly(container)
    }

    override fun onSaveInstanceState(): Parcelable? {
        return saveInstanceState(super.onSaveInstanceState())
    }

    private fun saveInstanceState(state: Parcelable?): Parcelable? {
        return Bundle().apply {
            putParcelable("SUPER_STATE_KEY", state)
            putSparseParcelableArray("SPARSE_STATE_KEY", saveChildViewStates())
        }
    }

    private fun saveChildViewStates(): SparseArray<Parcelable> {
        val childViewStates = SparseArray<Parcelable>()
        children.forEach { child -> child.saveHierarchyState(childViewStates) }
        return childViewStates
    }

    override fun onRestoreInstanceState(state: Parcelable?) {
        super.onRestoreInstanceState(restoreInstanceState(state))
    }

    private fun restoreInstanceState(state: Parcelable?): Parcelable? {
        var newState = state
        if (newState is Bundle) {
            newState = if (Build.VERSION.SDK_INT >= TIRAMISU) {
                val childrenState = newState.getSparseParcelableArray<Parcelable>("SPARSE_STATE_KEY", SavedState::class.java)
                childrenState?.let { restoreChildViewStates(it) }
                newState.getParcelable("SUPER_STATE_KEY", SavedState::class.java)
            } else {
                val childrenState = newState.getSparseParcelableArray<Parcelable>("SPARSE_STATE_KEY")
                childrenState?.let { restoreChildViewStates(it) }
                newState.getParcelable("SUPER_STATE_KEY")
            }
        }
        return newState
    }

    private fun restoreChildViewStates(childViewStates: SparseArray<Parcelable>) {
        children.forEach { child -> child.restoreHierarchyState(childViewStates) }
    }

    internal class SavedState : BaseSavedState {

        constructor(source: Parcel?, loader: ClassLoader?) : super(source, loader)
        constructor(superState: Parcelable?) : super(superState)

        companion object {

            @JvmField
            val CREATOR: Parcelable.ClassLoaderCreator<SavedState> = object :
                Parcelable.ClassLoaderCreator<SavedState> {
                override fun createFromParcel(source: Parcel): SavedState = SavedState(source, null)

                override fun newArray(size: Int): Array<SavedState?> = arrayOfNulls(size)

                override fun createFromParcel(source: Parcel, loader: ClassLoader): SavedState =
                    SavedState(source, loader)
            }
        }
    }

}
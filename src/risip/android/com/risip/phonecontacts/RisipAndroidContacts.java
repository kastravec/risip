package com.risip.phonecontacts;

import org.qtproject.qt5.android.bindings.QtActivity;
import android.os.Bundle;
import android.provider.ContactsContract;
import android.content.ContentResolver;
import android.database.Cursor;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.Manifest;
import android.content.pm.PackageManager;
//import android.support.v4.content.ContextCompat;
//import android.support.v4.app.ActivityCompat;

public class AndroidContacts extends QtActivity
{
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        Log.d("AContacts","On create called");
        super.onCreate(savedInstanceState);
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String permissions[], int[] grantResults) {
        Log.d("AContacts", "Permission result");
        switch (requestCode) {
            case 1: {
                // If request is cancelled, the result arrays are empty.
                if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    Log.d("AContacts", "Permission was granted");
                    // Granted
                    readContacts();
                } else {
                    Log.d("AContacts", " Permission was denied");
                }
                return;
            }
        }
    }

    public String[] readContacts()
    {
        String[] contacts;
        Log.d("AContacts"," Reading contacts");
        ContentResolver cr = getContentResolver();
        // Read Contacts
        Cursor c = cr.query(ContactsContract.Contacts.CONTENT_URI, null, null, null, null);
        int i = 0;
        if (c.getCount() > 0) {
            contacts = new String[c.getCount()];
            while (c.moveToNext()) {
                String id = c.getString(c.getColumnIndex(ContactsContract.Contacts._ID));
                String name = c.getString(c.getColumnIndex(ContactsContract.Contacts.DISPLAY_NAME));
                String newContact = String.format("{\"id\":\"%s\",\"name\":\"%s\"", id, name);

                // Iterate all the phone no's for this contact.
                Cursor numberCur = cr.query(ContactsContract.CommonDataKinds.Phone.CONTENT_URI, null
                                    ,ContactsContract.CommonDataKinds.Phone.CONTACT_ID +" = ?",new String[] {id},null);
                while (numberCur.moveToNext()) {
                    String label = numberCur.getString(numberCur.getColumnIndex(ContactsContract.CommonDataKinds.Phone.LABEL));
                    String phNo = numberCur.getString(numberCur.getColumnIndex(ContactsContract.CommonDataKinds.Phone.NUMBER));
                    int type = numberCur.getInt(numberCur.getColumnIndex(ContactsContract.CommonDataKinds.Phone.TYPE));
                    if (type != ContactsContract.CommonDataKinds.Phone.TYPE_CUSTOM) {
                        label = getResources().getString(ContactsContract.CommonDataKinds.Phone.getTypeLabelResource(type));
                    }
                    newContact = newContact.concat(String.format(",\"%s\":\"%s\"", label, phNo));
                }
                newContact = newContact.concat("}");
                Log.d("AContacts", newContact);
                contacts[i] = newContact;
                i++;
            }
        } else {
            contacts = new String[1];
            contacts[0] = "No Contacts";
        }
        return contacts;
    }

    public void getPhoneContacts()
    {
//        if (ContextCompat.checkSelfPermission(this, Manifest.permission.READ_CONTACTS) != PackageManager.PERMISSION_GRANTED) {

//            // Should we show an explanation?
//            if (ActivityCompat.shouldShowRequestPermissionRationale(this, Manifest.permission.READ_CONTACTS)) {
//                Log.d("AContacts"," Explain denial");
//                // TODO: Notify to explain.
//            } else {
//                // 1st timer, request the permission.
//                Log.d("AContacts","Requesting permission");
//                ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.READ_CONTACTS},1);
//            }
//        } else {
//            Log.d("CONTACTS"," Already Have permission");
//            readContacts();
//        }
    }

}

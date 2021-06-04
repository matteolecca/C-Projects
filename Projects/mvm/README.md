<h1>HOMOPHONES</h1>
<section>
<p><strong>
        A homophone is a word that is pronounced the same (to varying extent) as another word but differs in meaning
    </strong>
</p>
<h2>The program</h2>
<p>
    This little programs takes as input a single word and a number representing the number of 
    syllables, then checks for the relative homophones contained into the file cmudict.txt.
</p>
<h4>Example</h4>
<p>
   ./homophones -n 2 laboy <br/>
   <br/>
    LABOY = B OY1<br/>
      |<br/>
      V<br/>
- LABOY - DEBOY - BOY - BOYE 
</p>
</section>
<section>
<h3>Run the program</h3>
<p>To run the program:</p>
<ul>
    <li>Download the entire <strong>mvm</strong> folder.</li>
    <li>From the terminal navigate to the folder location.</li>
    <li>Run <strong>make -f hom.mk run</strong> to run the example above.</li> 
    <li>To check other words run <strong>./homophones -n ${number of syllables} ${word}</strong>. <br/>
    </li>

</ul>
</section>